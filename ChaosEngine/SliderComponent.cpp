#include "Application.h"
#include "SDL.h"
#include "SliderComponent.h"

SliderComponent::SliderComponent(int id, const char* text)
{
	name = "Slider Component";
	type = ComponentType::UI_SLIDER;
	this->text = text;
	value = 0;
	minValue = 0;
	maxValue = 100;
	drawRect = false;
	state = State::NORMAL;
	barProgres = 0.0f;
	completed = false;
	thePlane = App->editor->planes[App->editor->planes.size() - 1];
}

SliderComponent::~SliderComponent()
{
	text.clear();
}

void SliderComponent::Update()
{
	barProgres += 0.001;
	if (!completed) {


		if (barProgres < 0.5f)
		{
			thePlane->texCoords[4] = (0.5 - barProgres);
			thePlane->texCoords[6] = (0.5 - barProgres);
		}
		else if (barProgres >= 1) {
			completed = true;
		}
		else if (barProgres >= 0.5f) {

			float aux = barProgres - 0.5;
			thePlane->texCoords[0] = (1 - aux);
			thePlane->texCoords[2] = (1 - aux);
		}
		glDeleteBuffers(thePlane->texCoords.size() * sizeof(GLfloat), &thePlane->TBO);

		glGenBuffers(1, &thePlane->TBO);
		glBindBuffer(GL_ARRAY_BUFFER, thePlane->TBO);
		glBufferData(GL_ARRAY_BUFFER, thePlane->texCoords.size() * sizeof(GLfloat), thePlane->texCoords.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
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
