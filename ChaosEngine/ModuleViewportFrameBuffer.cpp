#include "Application.h"
#include "ModuleViewportFrameBuffer.h"
#include "Globals.h"
#include "ModuleWindow.h"
#include <string>
#include "imgui.h"
#include "imgui_internal.h"
#include "GL/glew.h"
#include <gl/GL.h>
#include"CameraComponent.h"
ModuleViewportFrameBuffer::ModuleViewportFrameBuffer(Application* app, bool start_enabled) : Module(app, start_enabled)
{

	//show_viewport_window = true;
}

ModuleViewportFrameBuffer::~ModuleViewportFrameBuffer()
{
	//glDeleteFramebuffers(1, &frameBuffer);
	//glDeleteTextures(1, &texture);
	//glDeleteRenderbuffers(1, &renderBufferoutput);
}

bool  ModuleViewportFrameBuffer::Init()
{
	return true;
}

bool ModuleViewportFrameBuffer::Start()
{
	bool ret = false;

	

	return true;
}
void ModuleViewportFrameBuffer::enableStart(ComponentCamera* camera) {
	
	if (camera->frameBuffer > 0)
	{
		glDeleteFramebuffers(1, &camera->frameBuffer);
		glDeleteTextures(1, &camera->texture);
		glDeleteRenderbuffers(1, &camera->renderBufferoutput);
	}

	glGenFramebuffers(1, &camera->frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, camera->frameBuffer);

	glGenTextures(1, &camera->texture);
	glBindTexture(GL_TEXTURE_2D, camera->texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, camera->size.x, camera->size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, camera->texture, 0);

	glGenTextures(1, &camera->renderBufferoutput);
	glBindTexture(GL_TEXTURE_2D, camera->renderBufferoutput);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, camera->size.x, camera->size.y);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, camera->renderBufferoutput, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
update_status ModuleViewportFrameBuffer::PreUpdate(float dt)
{
	

	return UPDATE_CONTINUE;
}
void ModuleViewportFrameBuffer::pre(ComponentCamera* camera) {
	Bind(camera);
	glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void ModuleViewportFrameBuffer::post(ComponentCamera* camera) {
	UnBind();
}
update_status ModuleViewportFrameBuffer::PostUpdate(float dt)
{
	//UnBind();

	return UPDATE_CONTINUE;
}

bool ModuleViewportFrameBuffer::CleanUp()
{
	/*texture ? glDeleteTextures(1, &texture) : 0;
	frameBuffer ? glDeleteFramebuffers(1, &frameBuffer) : 0;
	renderBufferoutput ? glDeleteRenderbuffers(1, &renderBufferoutput) : 0;*/

	return true;
}

void ModuleViewportFrameBuffer::Resize(uint width, uint height, ComponentCamera* camera)
{
	camera->size.x = width;
	camera->size.y = height;
	enableStart(camera);
}

void ModuleViewportFrameBuffer::Bind(ComponentCamera* camera)
{
	glBindFramebuffer(GL_FRAMEBUFFER, camera->frameBuffer);
	glViewport(0, 0, camera->size.x, camera->size.y);
}

void ModuleViewportFrameBuffer::UnBind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
