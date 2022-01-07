#include "Application.h"
#include "SDL.h"
#include "CheckboxComponent.h"

CheckboxComponent::CheckboxComponent(int id, const char* text)
{
	name = "CheckBox Component";
	type = ComponentType::UI_CHECKBOX;
	this->text = text;
	drawRectangle = false;
	state = State::NORMAL;
}

CheckboxComponent::~CheckboxComponent()
{
	text.clear();
}

void CheckboxComponent::Update()
{
	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
	{
		if (drawRectangle == false)
		{
			drawRectangle = true;
		}
		else
		{
			drawRectangle = false;
		}
	}
	if (state != State::DISABLED)
	{
		int mouseX = App->input->GetMouseX();
		int mouseY = App->input->GetMouseY();

		// Check collision between mouse and button bounds
		if (App->userInterface->focusedGameObject == owner)
		{
			/*if (state != UIControlState::FOCUSED && state != UIControlState::PRESSED)
			{
				App->audio->PlayFx(focusedFX);
			}*/
			state = State::FOCUSED;

			if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
			{
				state = State::PRESSED;
			}

			if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP)
			{
				checked = !checked;
				//NotifyObserver();
			}
		}
		else state = State::NORMAL;
	}
}

void CheckboxComponent::Draw()
{
	/*SDL_Rect rect;
	if (id == 8)
	{
		if (App->win->fullScreenWindow == true)
		{
			checked = true;
		}
		else
		{
			checked = false;
		}
		rect = { 297,1360,91,96 };
	}
	if (id == 9)
	{
		if (App->vsync == true)
		{
			checked = true;
		}
		else
		{
			checked = false;
		}
		rect = { 297,1360,91,96 };
	}*/
	// Draw the right button depending on state
	switch (state)
	{
	case State::DISABLED:
	{
		/*if (checked) 	app->render->DrawTexture(textureButtons, bounds.x, bounds.y, &rect8);
		else 	app->render->DrawTexture(textureButtons, bounds.x, bounds.y, &rect4);
		app->render->DrawTexture(textureButtons, bounds.x + 95, bounds.y + 15, &rect9);
		if (drawRectangle == true) app->render->DrawRectangle(bounds, 0, 255, 0, 255);*/
	} break;
	case State::NORMAL:
	{
		/*if (checked) 	app->render->DrawTexture(textureButtons, bounds.x, bounds.y, &rect5);
		else 	app->render->DrawTexture(textureButtons, bounds.x, bounds.y, &rect);
		app->render->DrawTexture(textureButtons, bounds.x + 95, bounds.y + 15, &rect9);
		if (drawRectangle == true) app->render->DrawRectangle(bounds, 0, 255, 0, 255);*/
	} break;
	case State::FOCUSED:
	{
		/*if (checked) 	app->render->DrawTexture(textureButtons, bounds.x, bounds.y, &rect6);
		else 	app->render->DrawTexture(textureButtons, bounds.x, bounds.y, &rect2);
		app->render->DrawTexture(textureButtons, bounds.x + 95, bounds.y + 15, &rect9);
		if (drawRectangle == true) app->render->DrawRectangle(bounds, 255, 255, 0, 255);*/
	} break;
	case State::PRESSED:
	{
		/*if (checked) 	app->render->DrawTexture(textureButtons, bounds.x, bounds.y, &rect7);
		else 	app->render->DrawTexture(textureButtons, bounds.x, bounds.y, &rect3);
		app->render->DrawTexture(textureButtons, bounds.x + 95, bounds.y + 15, &rect9);
		if (drawRectangle == true) app->render->DrawRectangle(bounds, 255, 0, 0, 255);*/
	} break;
	default:
		break;
	}
}

void CheckboxComponent::OnEditor(int i)
{
	// General variables
	static float multiplier = 1;
	static float fadeDuration = 0.1f;
	// Colors
	static float normalColor[3] = { 255,255,255 };
	static float pressedColor[3] = { 255,255,255 };
	static float selectedColor[3] = { 255,255,255 };
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

