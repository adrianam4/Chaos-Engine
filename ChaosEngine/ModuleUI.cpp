#include "Globals.h"
#include "Application.h"
#include "ModuleUI.h"
#include "CameraComponent.h"
#include <stack>
#include <ft2build.h>

#include <GL/gl.h>

#include FT_FREETYPE_H 


Shader::Shader()
{
	// 1. retrieve the vertex/fragment source code from filePath
	std::string vertexCode = "#version 330 core\n  in vec4 vertex;\n out vec2 TexCoords;\n  uniform mat4 projection;\n  void main() {\n  gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);\n  TexCoords = vertex.zw; };";
	std::string fragmentCode = "#version 330 core\n  in vec2 TexCoords;\n  out vec4 color;\n  uniform sampler2D text;\n  uniform vec3 textColor;\n  void main() {\n  vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);\n  color = vec4(textColor, 1.0) * sampled;};"; //color = vec4(textColor, 1.0) * sampled;};";

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
	// 2. compile shaders
	unsigned int vertex, fragment;
	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	CheckCompileErrors(vertex, "VERTEX");
	// fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	CheckCompileErrors(fragment, "FRAGMENT");

	// shader Program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);

	glLinkProgram(ID);
	CheckCompileErrors(ID, "PROGRAM");
	// delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertex);
	glDeleteShader(fragment);


}
// activate the shader
// ------------------------------------------------------------------------
void Shader::Use()
{
	glUseProgram(ID);
}
// deactivate the shader
// ------------------------------------------------------------------------
void Shader::StopUse()
{
	glUseProgram(0);
}

// utility function for checking shader compilation/linking errors.
// ------------------------------------------------------------------------
void Shader::CheckCompileErrors(GLuint shader, std::string type)
{
	GLint success;
	GLchar infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}


////////////////////////////////////////////////


ModuleUI::ModuleUI(Application* app, bool startEnabled) : Module(app, startEnabled)
{
	focusedGameObject = nullptr;
}

ModuleUI::~ModuleUI()
{
}

bool ModuleUI::Start()
{
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		//LOG("ERROR::FREETYPE: Could not init FreeType Library");
		return false;
	}

	FT_Face face;
	if (FT_New_Face(ft, "Assets/Fonts/OpenSans-Bold.ttf", 0, &face))
	{
		//LOG("ERROR::FREETYPE: Failed to load font");
		return false;
	}
	else {
		// set size to load glyphs as
		FT_Set_Pixel_Sizes(face, 0, 48);

		// disable byte-alignment restriction
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		// load first 128 characters of ASCII set
		for (unsigned char c = 0; c < 128; c++)
		{
			// Load character glyph 
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
				continue;
			}
			// generate texture
			unsigned int texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
			);
			// set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// now store character for later use
			Character character = {
				texture,
				IVec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				IVec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				static_cast<unsigned int>(face->glyph->advance.x)
			};
			characters.insert(std::pair<char, Character>(c, character));
		}
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	// destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);


	// configure VAO/VBO for texture quads
	// -----------------------------------
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	shader = new Shader();
	return true;
}
void ModuleUI::RenderText(std::string text, float x, float y, float scale, float3 color)
{
	// activate corresponding render state	
	shader->Use();
	Frustum frustum;
	frustum.pos = App->camera->editorCam->frustum.pos;
	frustum.front = App->camera->editorCam->frustum.front; //COGED EL FRONT DE LA CAMARA DE JUEGO
	frustum.up = App->camera->editorCam->frustum.up; //COGED EL UP DE LA CAMARA DE JUEGO
	frustum.type = OrthographicFrustum;
	frustum.orthographicHeight = 500;//PONER EL TAMAÑO DEL VIEWPORT DONDE QUERAIS PINTAR
	frustum.orthographicWidth = 500;//PONER EL TAMAÑO DEL VIEWPORT DONDE QUERAIS PINTAR
	frustum.nearPlaneDistance = 0.1;
	frustum.farPlaneDistance = 1000000.f;

	auto p = frustum.ProjectionMatrix();
	glUniform3f(glGetUniformLocation(shader->ID, "textColor"), color.x, color.y, color.z);
	glUniformMatrix4fv(glGetUniformLocation(shader->ID, "projection"), 1, GL_TRUE, p.Transposed().ptr());
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);

	// iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = characters[*c];

		float xpos = x + ch.Bearing.x * scale;
		float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;
		// update VBO for each character
		float vertices[6][4] = {
			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },

			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 0.0f }
		};
		// render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	shader->StopUse();
}
update_status ModuleUI::PreUpdate(float dt)
{
	float2 mousePos = { (float)App->input->GetMouseX() ,(float)App->input->GetMouseY() };
	float2 mPos = { ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y };
	float4 viewport = App->editor->viewport;

	if (mousePos.x > viewport.x && mousePos.x < viewport.x + viewport.z && mousePos.y > viewport.y && mousePos.y < viewport.y + viewport.w)
	{
		float2 fMousePos = { mPos.x - viewport.x , mPos.y - viewport.y };
		float normalized_x = Lerp(-1, 1, fMousePos.x / viewport.z);
		float normalized_y = Lerp(1, -1, fMousePos.y / viewport.w);

		App->camera->GameCam->frustum.GetCornerPoints(&corners[0]);
		myRay = App->camera->GameCam->frustum.UnProjectLineSegment(normalized_x, normalized_y);

		for (int i = 0; i < App->scene->gameObjects.size(); i++)
		{
			GameObject* go = App->scene->gameObjects[i];
			for (int j = 0; j < go->boundingBoxes.size(); j++)
			{
				if (bool hit = myRay.Intersects(*go->aabb[j]))
				{
					if (go->SearchComponent(go, ComponentType::UI_BUTTON) != -1 || go->SearchComponent(go, ComponentType::UI_CHECKBOX) != -1 || go->SearchComponent(go, ComponentType::UI_IMAGE) != -1 ||
						go->SearchComponent(go, ComponentType::UI_INPUTBOX) != -1 || go->SearchComponent(go, ComponentType::UI_SLIDER) != -1)
					{
						hitObjs.push_back(go);
					}
				}
			}
		}

		if (hitObjs.size() > 0)
		{
			std::vector<float> distance;
			float nearDist = 500.0f;
			int nearObj = 0;
			for (int i = 0; i < hitObjs.size(); ++i)
			{
				int myComp = hitObjs[i]->SearchComponent(hitObjs[i], ComponentType::TRANSFORM2D);
				float3 distnceVec = hitObjs[i]->components[myComp]->position - App->camera->GameCam->frustum.pos;
				float finalDistance = math::Sqrt((distnceVec.x * distnceVec.x) + (distnceVec.y * distnceVec.y) + (distnceVec.z * distnceVec.z));
				distance.push_back(finalDistance);
			}
			for (int i = 0; i < distance.size(); i++)
			{
				if (distance[i] < nearDist)
				{
					nearDist = distance[i];
					nearObj = i;
				}
			}
			focusedGameObject = hitObjs[nearObj];
			hitObjs.clear();
		}
		else
			focusedGameObject = nullptr;
	}

	return UPDATE_CONTINUE;
}

update_status ModuleUI::Update(float dt)
{
	// Update All UI Components
	/*if (App->gameMode)
	{*/
	for (int i = 0; i < App->scene->gameObjects.size(); i++)
	{
		GameObject* go = App->scene->gameObjects[i];

		int button = go->SearchComponent(go, ComponentType::UI_BUTTON);
		int checkbox = go->SearchComponent(go, ComponentType::UI_CHECKBOX);
		int image = go->SearchComponent(go, ComponentType::UI_IMAGE);
		int inputbox = go->SearchComponent(go, ComponentType::UI_INPUTBOX);
		int slider = go->SearchComponent(go, ComponentType::UI_SLIDER);

		if (button != -1)
			go->components[button]->Update();
		if (checkbox != -1)
			go->components[checkbox]->Update();
		if (image != -1)
			go->components[image]->Update();
		if (inputbox != -1)
			go->components[inputbox]->Update();
		if (slider != -1)
			go->components[slider]->Update();
	}
	//}
	
	return UPDATE_CONTINUE;
}

update_status ModuleUI::PostUpdate(float dt)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);

	glPushMatrix();

	App->viewportBuffer->Bind(App->camera->editorCam);
	RenderText("Aroca", 0, 0, 1.f, float3(1.f, 0.f, 0.f));

	glEnable(GL_DEPTH_TEST);
	glPopMatrix();
	App->viewportBuffer->UnBind();

	return UPDATE_CONTINUE;
}

bool ModuleUI::CleanUp()
{
	return true;
}