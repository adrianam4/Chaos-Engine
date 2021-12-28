#include "Application.h"
#include "SDL.h"
#include "ButtonComponent.h"

ButtonComponent::ButtonComponent(int id, SDL_Rect bounds, std::string text, SDL_Texture* textureButton)
{
	name = "Button Component";
	this->bounds = bounds;
	this->text = text;
	texture = textureButton;
	state = State::NORMAL;
}

ButtonComponent::~ButtonComponent()
{
	text.clear();
	delete texture;
}

void ButtonComponent::Update()
{
	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
	{
		if (buttonsColliders == false)
		{
			buttonsColliders = true;
		}
		else
		{
			buttonsColliders = false;
		}
	}

	if (state != State::DISABLED)
	{
		int mouseX = App->input->GetMouseX();
		int mouseY = App->input->GetMouseY();

		// Check collision between mouse and button bounds
		if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) &&
			(mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)))
		{
			if (state != State::FOCUSED && state != State::PRESSED)
			{
				/*app->audio->PlayFx(focusedFX);*/
			}
			state = State::FOCUSED;

			if (App->input->GetKey(SDL_BUTTON_LEFT) == KEY_REPEAT)
			{
				state = State::PRESSED;
			}

			// If mouse button pressed -> Generate event!
			if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP)
			{
				/*App->audio->PlayFx(clickedFX);
				NotifyObserver();*/
			}
		}
		else state = State::NORMAL;
	}
}

void ButtonComponent::Draw()
{
	switch (state)
	{
	case State::DISABLED:
		/*App->render->DrawTexture(textureButtons, bounds.x, bounds.y, &rect);
		if (buttonsColliders == true)
		{
			app->render->DrawRectangle(bounds, 0, 255, 0, 255);
		}*/
		break;

	case State::NORMAL:
		/*App->render->DrawTexture(textureButtons, bounds.x, bounds.y, &rect2);
		if (buttonsColliders == true)
		{
			app->render->DrawRectangle(bounds, 0, 255, 0, 255);
		}*/
		break;

	case State::FOCUSED:
		/*App->render->DrawTexture(textureButtons, bounds.x, bounds.y, &rect3);
		if (buttonsColliders == true)
		{
			app->render->DrawRectangle(bounds, 255, 255, 0, 255);
		}*/
		break;

	case State::PRESSED:
		/*App->render->DrawTexture(textureButtons, bounds.x, bounds.y, &rect4);
		if (buttonsColliders == true)
		{
			app->render->DrawRectangle(bounds, 0, 255, 255, 255);
		}*/
		break;

	case State::SELECTED:
		/*App->render->DrawRectangle(bounds, 0, 255, 0, 255);*/
		break;

	default:
		break;
	}
}

void ButtonComponent::OnEditor(int i)
{
	// General variables
	static float multiplier = 1;
	static float fadeDuration = 0.1f;
	// Colors
	static float normalColor[3] = {255,255,255};
	static float pressedColor[3] = {255,255,255};
	static float selectedColor[3] = {255,255,255};
	static float disabledColor[3] = { 255,255,255 };
	// Manage if colors are being edited or not
	static bool normalEditable = false;
	static bool pressedEditable = false;
	static bool selectedEditable = false;
	static bool disabledEditable = false;

	ImGui::Checkbox("Interactable", &active);

	ImGui::Text("Normal Color"); ImGui::SameLine(); 
	if (ImGui::ColorButton("Normal Color", ImVec4(normalColor[0], normalColor[1], normalColor[2], 255)))
		normalEditable = !normalEditable;

	ImGui::Text("Pressed Color"); ImGui::SameLine();
	if (ImGui::ColorButton("Pressed Color", ImVec4(pressedColor[0], pressedColor[1], pressedColor[2], 255)))
		pressedEditable = !pressedEditable;

	ImGui::Text("Selected Color"); ImGui::SameLine();
	if (ImGui::ColorButton("Selected Color", ImVec4(selectedColor[0], selectedColor[1], selectedColor[2], 255)))
		selectedEditable = !selectedEditable;

	ImGui::Text("Disabled Color"); ImGui::SameLine();
	if (ImGui::ColorButton("Disabled Color", ImVec4(disabledColor[0], disabledColor[1], disabledColor[2], 255)))
		disabledEditable = !disabledEditable;

	if (normalEditable)
	{
		ImGui::ColorPicker3("Normal Color", normalColor);
	}
	if (pressedEditable)
	{
		ImGui::ColorPicker3("Pressed Color", pressedColor);
	}
	if (selectedEditable)
	{
		ImGui::ColorPicker3("Selected Color", selectedColor);
	}
	if (disabledEditable)
	{
		ImGui::ColorPicker3("Disabled Color", disabledColor);
	}
	
	ImGui::SliderFloat("Color Multiplier", &multiplier, 1, 5);
	ImGui::InputFloat("Fade Duration", &fadeDuration);
}
