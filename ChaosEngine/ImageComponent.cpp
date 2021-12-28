#include "Application.h"
#include "SDL.h"
#include "ImageComponent.h"

ImageComponent::ImageComponent(int id, SDL_Rect bounds, std::string text, SDL_Texture* textureButton)
{
	name = "Image Component";
	this->bounds = bounds;
	this->text = text;
	texture = textureButton;
	state = State::NORMAL;
}

ImageComponent::~ImageComponent()
{
	text.clear();
	delete texture;
}

void ImageComponent::Update()
{
}

void ImageComponent::Draw()
{
}

void ImageComponent::OnEditor(int i)
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
