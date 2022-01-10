#include "InputBoxComponent.h"
#include "SDL.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "CameraComponent.h"

InputBoxComponent::InputBoxComponent(int id, char* text)
{
	name = "InputBox Component";
	type = ComponentType::UI_INPUTBOX;
	//this->text = text;
	text = "Ejemplo";
	drawRect = false;
	state = State::NORMAL;
	aux.setText("Default", 0, 0, 0.5, { 255,255,255 });
}

InputBoxComponent::~InputBoxComponent()
{
	
}

void InputBoxComponent::Update()
{
	aux.SetOnlyPosition(float2(FromWorldToScreen().x, FromWorldToScreen().y));

	if (state != State::DISABLED)
	{
		// Check collision between mouse and button bounds
		if (App->userInterface->focusedGameObject == owner)
		{
			state = State::FOCUSED;
			color = focusedColor;
			if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
			{
				state = State::PRESSED;
				color = pressedColor;
				//NotifyObserver();
			}
		}
		else
		{
			state = State::NORMAL;
			color = normalColor;
		}
		if (App->userInterface->UIGameObjectSelected == owner)
		{
			state = State::SELECTED;
			color = selectedColor;
			if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
				OnClick();
		}
	}
	else
		color = disabledColor;
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
	static bool textColorEditable = false;

	// Manage if colors are being edited or not
	static bool normalEditable = false;
	static bool pressedEditable = false;
	static bool focusedEditable = false;
	static bool disabledEditable = false;
	static bool selectedEditable = false;

	ImGui::Checkbox("Active", &active);

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

	ImGui::Separator();

	ImGui::Text("Text Color"); ImGui::SameLine();
	if (ImGui::ColorButton("Text Color", ImVec4(textColor.r, textColor.g, textColor.b, textColor.a))) {
		
		textColorEditable = !textColorEditable;
	}
	aux.setOnlyColor({ textColor.r, textColor.g, textColor.b });

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
	if (textColorEditable)
	{
		ImGui::ColorPicker3("Text Color", &textColor);
	}

	ImGui::InputText("Text", text, IM_ARRAYSIZE(text));
	ImGui::DragFloat("Font Size", &fontScale,0.1,0,10);
	aux.setOnlyText(text);
}

void InputBoxComponent::OnClick()
{
}

float2 InputBoxComponent::FromWorldToScreen()
{
	ComponentTransform2D* transform = owner->getTransform2D();
	float4 worldPosition = { transform->position.x, transform->position.y, transform->position.z, 1 };
	float4 screenPos = App->camera->GameCam->frustum.ProjectionMatrix() * App->camera->GameCam->frustum.ViewMatrix() * worldPosition;
	return { 0, 0 }; // Mal
}
