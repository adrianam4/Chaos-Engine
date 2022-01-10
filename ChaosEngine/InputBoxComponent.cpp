#include "InputBoxComponent.h"
#include "SDL.h"
#include "Application.h"

InputBoxComponent::InputBoxComponent(int id, char* text)
{
	name = "InputBox Component";
	type = ComponentType::UI_INPUTBOX;
	//this->text = text;
	text = "Ejemplo";
	drawRect = false;
	state = State::NORMAL;
}

InputBoxComponent::~InputBoxComponent()
{
	
}

void InputBoxComponent::Update()
{
	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
	{
		if (drawRect == false)
		{
			drawRect = true;
		}
		else
		{
			drawRect = false;
		}
	}

	if (state != State::DISABLED)
	{
		int mouseX = App->input->GetMouseX();
		int mouseY = App->input->GetMouseY();

		// Check collision between mouse and button bounds
		if (App->userInterface->focusedGameObject == owner)
		{
			state = State::FOCUSED;
			if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
			{
				state = State::PRESSED;

				//NotifyObserver();
			}
		}
		else state = State::NORMAL;

	}
}

void InputBoxComponent::Draw()
{
	MyPlane* planeToDraw = nullptr;
	int auxId = owner->id;

	for (int i = 0; i < App->editor->planes.size(); i++)
		if (App->editor->planes[i]->id == auxId) planeToDraw = App->editor->planes[i];

	glAlphaFunc(GL_GREATER, 0.5);
	glEnable(GL_ALPHA_TEST);

	glColor4f(color.r, color.g, color.b, color.a);

	planeToDraw->DrawPlane();

	glDisable(GL_ALPHA_TEST);
	glColor3f(255, 255, 255);
}

void InputBoxComponent::OnEditor(int i)
{
	// Manage if colors are being edited or not
	static bool normalEditable = false;
	static bool textColorEditable = false;

	ImGui::Checkbox("Active", &active);

	ImGui::Text("Background Color"); ImGui::SameLine();
	if (ImGui::ColorButton("Background Color", ImVec4(color.r, color.g, color.b, color.a)))
		normalEditable = !normalEditable;

	ImGui::Text("Text Color"); ImGui::SameLine();
	if (ImGui::ColorButton("Text Color", ImVec4(textColor.r, textColor.g, textColor.b, textColor.a)))
		textColorEditable = !textColorEditable;

	if (normalEditable)
	{
		ImGui::ColorPicker3("Background Color", &color);
	}
	if (textColorEditable)
	{
		ImGui::ColorPicker3("Text Color", &textColor);
	}

	ImGui::InputText("Text", text, IM_ARRAYSIZE(text));
	ImGui::DragFloat("Font Size", &fontScale,0.1,0,10);
}
