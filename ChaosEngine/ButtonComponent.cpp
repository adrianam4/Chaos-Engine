#include "Application.h"
#include "SDL.h"
#include "ButtonComponent.h"

ButtonComponent::ButtonComponent(int id, SDL_Rect bounds, std::string text, SDL_Texture* textureButton)
{
	name = "Button Component";
	type = ComponentType::UI_BUTTON;

	this->bounds = &bounds;
	this->text = text;

	disabledTexture = textureButton;
	normalTexture = textureButton;
	focusedTexture = textureButton;
	pressedTexture = textureButton;

	state = State::NORMAL;
}

ButtonComponent::~ButtonComponent()
{
	text.clear();
	delete normalTexture;
	delete disabledTexture;
	delete focusedTexture;
	delete pressedTexture;
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
		if ((mouseX > bounds->x) && (mouseX < (bounds->x + bounds->w)) &&
			(mouseY > bounds->y) && (mouseY < (bounds->y + bounds->h)))
		{
			if (state != State::FOCUSED && state != State::PRESSED)
			{
				/*app->audio->PlayFx(focusedFX);*/
			}
			state = State::FOCUSED;

			if (App->input->GetKey(SDL_BUTTON_LEFT) == KEY_REPEAT)
			{
				state = State::PRESSED;
				OnClick();
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
		break;
	case State::NORMAL:
		break;
	case State::FOCUSED:
		break;
	case State::PRESSED:
		break;
	case State::SELECTED:
		break;
	default:
		break;
	}
}

void ButtonComponent::OnEditor(int i)
{
	Update();

	// General variables
	static float multiplier = 1;
	static float fadeDuration = 0.1f;

	// Manage if colors are being edited or not
	static bool normalEditable = false;
	static bool pressedEditable = false;
	static bool selectedEditable = false;
	static bool disabledEditable = false;

	ImGui::Checkbox("Interactable", &active);

	ImGui::Text("Normal Color"); ImGui::SameLine(); 
	if (ImGui::ColorButton("Normal Color", ImVec4(normalColor.r, normalColor.g, normalColor.b, normalColor.a)))
		normalEditable = !normalEditable;

	ImGui::Text("Pressed Color"); ImGui::SameLine();
	if (ImGui::ColorButton("Pressed Color", ImVec4(pressedColor.r, pressedColor.g, pressedColor.b, pressedColor.a)))
		pressedEditable = !pressedEditable;

	ImGui::Text("Selected Color"); ImGui::SameLine();
	if (ImGui::ColorButton("Selected Color", ImVec4(focusedColor.r, focusedColor.g, focusedColor.b, focusedColor.a)))
		selectedEditable = !selectedEditable;

	ImGui::Text("Disabled Color"); ImGui::SameLine();
	if (ImGui::ColorButton("Disabled Color", ImVec4(disabledColor.r, disabledColor.g, disabledColor.b, disabledColor.a)))
		disabledEditable = !disabledEditable;

	if (normalEditable)
	{
		ImGui::ColorPicker3("Normal Color", &normalColor);
	}
	if (pressedEditable)
	{
		ImGui::ColorPicker3("Pressed Color", &pressedColor);
	}
	if (selectedEditable)
	{
		ImGui::ColorPicker3("Selected Color", &focusedColor);
	}
	if (disabledEditable)
	{
		ImGui::ColorPicker3("Disabled Color", &disabledColor);
	}
	
	ImGui::SliderFloat("Color Multiplier", &multiplier, 1, 5);
	ImGui::InputFloat("Fade Duration", &fadeDuration);
}

void ButtonComponent::OnClick()
{
	SDL_Quit();
}
