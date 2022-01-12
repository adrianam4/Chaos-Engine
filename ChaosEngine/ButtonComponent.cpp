#include "Application.h"
#include "SDL.h"
#include "ButtonComponent.h"

ButtonComponent::ButtonComponent(int id, std::string text)
{
	name = "Button Component";
	type = ComponentType::UI_BUTTON;
	this->text = text;
	state = State::NORMAL;
}

ButtonComponent::~ButtonComponent()
{
	text.clear();
}

void ButtonComponent::Update()
{
	if (!active)
		state = State::DISABLED;
	else
		state = State::NORMAL;

	if (state != State::DISABLED)
	{
		float2 mousePos = { (float)App->input->GetMouseX() ,(float)App->input->GetMouseY() };
		float2 mPos = { ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y };
		float4 viewport = App->editor->viewport;
		float2 fMousePos = { mPos.x - viewport.x , mPos.y - viewport.y };

		ComponentTransform2D* transform2D = owner->getTransform2D();

		if (mousePos.x > viewport.x && mousePos.x < viewport.x + viewport.z && mousePos.y > viewport.y && mousePos.y < viewport.y + viewport.w)
		{

			float posXMin = ((viewport.z / 2) + (transform2D->position.x)) - (transform2D->buttonWidth / 2);
			float posXMax = ((viewport.z / 2) + (transform2D->position.x)) + (transform2D->buttonWidth / 2);

			float posYMin = ((viewport.w / 2) + (transform2D->position.y)) - (transform2D->buttonHeight / 2);
			float posYMax = ((viewport.w / 2) + (transform2D->position.y)) + (transform2D->buttonHeight / 2);

			if (fMousePos.x > posXMin && fMousePos.x < posXMax && fMousePos.y > posYMin && fMousePos.y < posYMax) {

				state = State::FOCUSED;

				if (state != State::FOCUSED && state != State::PRESSED)
				{
					/*app->audio->PlayFx(focusedFX);*/
				}

				if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
				{
					state = State::PRESSED;
				}

				// If mouse button pressed -> Generate event!
				if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP)
				{
					OnClick();
				}
			}
		}
		else state = State::NORMAL;

		if (App->userInterface->UIGameObjectSelected == owner)
		{
			state = State::SELECTED;
			if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
				OnClick();
		}
	}
}

void ButtonComponent::Draw()
{
	MyPlane* planeToDraw = nullptr;
	int auxId = owner->id;

	for (int i = 0; i < App->editor->planes.size(); i++)
		if (App->editor->planes[i]->id == auxId) planeToDraw = App->editor->planes[i];
	
	glAlphaFunc(GL_GREATER, 0.5);
	glEnable(GL_ALPHA_TEST);

	switch (state)
	{
	case State::DISABLED:
		glColor4f(disabledColor.r, disabledColor.g, disabledColor.b, disabledColor.a);
		break;
	case State::NORMAL:
		glColor4f(normalColor.r, normalColor.g, normalColor.b, normalColor.a);
		break;
	case State::FOCUSED:
		glColor4f(focusedColor.r, focusedColor.g, focusedColor.b, focusedColor.a);
		break;
	case State::PRESSED:
		glColor4f(pressedColor.r, pressedColor.g, pressedColor.b, pressedColor.a);
		break;
	case State::SELECTED:
		glColor4f(selectedColor.r, selectedColor.g, selectedColor.b, selectedColor.a);
		break;
	default:
		break;
	}

	planeToDraw->DrawPlane2D();
	
	glDisable(GL_ALPHA_TEST);
	glColor3f(255, 255, 255);
}

void ButtonComponent::OnEditor(int i)
{
	// General variables
	static float multiplier = 1;
	static float fadeDuration = 0.1f;

	// Manage if colors are being edited or not
	static bool normalEditable = false;
	static bool pressedEditable = false;
	static bool focusedEditable = false;
	static bool disabledEditable = false;
	static bool selectedEditable = false;

	ImGui::Checkbox("Interactable", &active);

	ImGui::Text("Normal Color"); ImGui::SameLine(); 
	if (ImGui::ColorButton("Normal Color", ImVec4(normalColor.r, normalColor.g, normalColor.b, normalColor.a)))
		normalEditable = !normalEditable;

	ImGui::Text("Pressed Color"); ImGui::SameLine();
	if (ImGui::ColorButton("Pressed Color", ImVec4(pressedColor.r, pressedColor.g, pressedColor.b, pressedColor.a)))
		pressedEditable = !pressedEditable;

	ImGui::Text("Focused Color"); ImGui::SameLine();
	if (ImGui::ColorButton("Focused Color", ImVec4(focusedColor.r, focusedColor.g, focusedColor.b, focusedColor.a)))
		focusedEditable = !focusedEditable;

	ImGui::Text("Disabled Color"); ImGui::SameLine();
	if (ImGui::ColorButton("Disabled Color", ImVec4(disabledColor.r, disabledColor.g, disabledColor.b, disabledColor.a)))
		disabledEditable = !disabledEditable;

	ImGui::Text("Selected Color"); ImGui::SameLine();
	if (ImGui::ColorButton("Selected Color", ImVec4(selectedColor.r, selectedColor.g, selectedColor.b, selectedColor.a)))
		selectedEditable = !selectedEditable;

	if (normalEditable)
	{
		ImGui::ColorPicker3("Normal Color", &normalColor);
	}
	if (pressedEditable)
	{
		ImGui::ColorPicker3("Pressed Color", &pressedColor);
	}
	if (focusedEditable)
	{
		ImGui::ColorPicker3("Focused Color", &focusedColor);
	}
	if (disabledEditable)
	{
		ImGui::ColorPicker3("Disabled Color", &disabledColor);
	}
	if (selectedEditable)
	{
		ImGui::ColorPicker3("Selected Color", &selectedColor);
	}
	
	ImGui::SliderFloat("Color Multiplier", &multiplier, 1, 5);
	ImGui::InputFloat("Fade Duration", &fadeDuration);
}

void ButtonComponent::OnClick()
{
	
}
