#include "Application.h"
#include "SDL.h"
#include "SliderComponent.h"

SliderComponent::SliderComponent(int id, SDL_Rect bounds, const char* text, SDL_Texture* textureButton, SDL_Texture* textureSlider)
{
	name = "Slider Component";
	type = ComponentType::UI_SLIDER;
	this->bounds = bounds;
	this->text = text;
	value = 0;
	minValue = 0;
	maxValue = 100;
	//textureButtons = textureButton;
	texture = textureSlider;
	drawRect = false;
	state = State::NORMAL;
}

SliderComponent::~SliderComponent()
{
	text.clear();
	delete texture;
}

void SliderComponent::Update()
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
		if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) &&
			(mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)))
		{
			state = State::FOCUSED;
			if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
			{
				state = State::PRESSED;
				value = ((maxValue - minValue) * (mouseX - (float)(bounds.x + slider.w / 2))) / (float)(bounds.w - slider.w) + minValue;

				//NotifyObserver();

				if (slider.w > 0)
				{
					slider.x = mouseX - slider.w / 2;
				}
				else if (slider.w == 0)
				{
					//slider.w = sliderValue;
				}
			}
		}
		else state = State::NORMAL;

		if (value > maxValue)
		{
			value = maxValue;
		}

		else if (value < minValue)
		{
			value = minValue;
		}
	}
}

void SliderComponent::Draw()
{
	SDL_Rect rect = { 3486,3838,359,57 };
	SDL_Rect rect2 = { 3450,3955, 3.41 * value,46 };
	SDL_Rect rect3;

	switch (state)
	{
	case State::DISABLED:
		/*app->render->DrawTexture(textureSliders, bounds.x, bounds.y, &rect);
		app->render->DrawTexture(textureSliders, bounds.x + 8, bounds.y + 6, &rect2);
		app->render->DrawTexture(textureButtons, bounds.x - 95, bounds.y, &rect3);
		if (drawRect == true) app->render->DrawRectangle(bounds, 0, 255, 0, 255);
		*/
		break;
	case State::NORMAL:
		/*app->render->DrawTexture(textureSliders, bounds.x, bounds.y, &rect);
		app->render->DrawTexture(textureSliders, bounds.x + 8, bounds.y + 6, &rect2);
		app->render->DrawTexture(textureButtons, bounds.x - 95, bounds.y, &rect3);
		if (drawRect == true) app->render->DrawRectangle(bounds, 0, 255, 0, 255);
		*/
		break;
	case State::FOCUSED:
		/*app->render->DrawTexture(textureSliders, bounds.x, bounds.y, &rect);
		app->render->DrawTexture(textureSliders, bounds.x + 8, bounds.y + 6, &rect2);
		app->render->DrawTexture(textureButtons, bounds.x - 95, bounds.y, &rect3);
		if (drawRect == true) app->render->DrawRectangle(bounds, 255, 255, 0, 255);
		*/
		break;
	case State::PRESSED:
		/*app->render->DrawTexture(textureSliders, bounds.x, bounds.y, &rect);
		app->render->DrawTexture(textureSliders, bounds.x + 8, bounds.y + 6, &rect2);
		app->render->DrawTexture(textureButtons, bounds.x - 95, bounds.y, &rect3);
		if (drawRect == true) app->render->DrawRectangle(bounds, 255, 0, 0, 255);
		*/
		break;
	default:
		break;
	}
}

void SliderComponent::OnEditor(int i)
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
	ImGui::InputFloat("Min Value", &minValue);
	ImGui::InputFloat("Max Value", &maxValue);
	ImGui::SliderFloat("Value", &value, minValue, maxValue);
}
