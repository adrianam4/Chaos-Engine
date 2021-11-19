#include "Application.h"
#include "ModuleViewportFrameBuffer.h"
#include "Globals.h"
#include "ModuleWindow.h"
#include <string>
#include "imgui.h"
#include "imgui_internal.h"
#include "GL/glew.h"
#include <gl/GL.h>

ModuleViewportFrameBuffer::ModuleViewportFrameBuffer(Application* app, bool start_enabled) : Module(app, start_enabled)
{

	show_viewport_window = true;
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

	if (frameBuffer > 0)
	{
		glDeleteFramebuffers(1, &frameBuffer);
		glDeleteTextures(1, &texture);
		glDeleteRenderbuffers(1, &renderBufferoutput);
	}

	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x, size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

	glGenTextures(1, &renderBufferoutput);
	glBindTexture(GL_TEXTURE_2D, renderBufferoutput);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, size.x, size.y);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, renderBufferoutput, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

update_status ModuleViewportFrameBuffer::PreUpdate(float dt)
{
	Bind();
	glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	return UPDATE_CONTINUE;
}

update_status ModuleViewportFrameBuffer::PostUpdate(float dt)
{
	UnBind();

	return UPDATE_CONTINUE;
}

bool ModuleViewportFrameBuffer::CleanUp()
{
	texture ? glDeleteTextures(1, &texture) : 0;
	frameBuffer ? glDeleteFramebuffers(1, &frameBuffer) : 0;
	renderBufferoutput ? glDeleteRenderbuffers(1, &renderBufferoutput) : 0;

	return true;
}

void ModuleViewportFrameBuffer::Resize(uint width, uint height)
{
	size.x = width;
	size.y = height;
	Start();
}

void ModuleViewportFrameBuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT); // ¿?¿?¿?
}

void ModuleViewportFrameBuffer::UnBind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
