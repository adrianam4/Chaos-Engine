#include "Application.h"
#include "SDL.h"
#include "SliderComponent.h"

SliderComponent::SliderComponent(int id, std::string text)
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
	if (!active)
		state = State::DISABLED;
	else
		state = State::NORMAL;

	if (state != State::DISABLED)
	{
		if (App->userInterface->focusedGameObject == owner)
		{
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
		else state = State::NORMAL;

		if (App->userInterface->UIGameObjectSelected == owner)
		{
			state = State::SELECTED;
			if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
				OnClick();
		}
	}

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

	planeToDraw->DrawPlane();

	glDisable(GL_ALPHA_TEST);
	glColor3f(255, 255, 255);
}

void SliderComponent::OnEditor(int i)
{
	// Colors
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

	ImGui::InputFloat("Min Value", &minValue);
	ImGui::InputFloat("Max Value", &maxValue);
	ImGui::SliderFloat("Value", &value, minValue, maxValue);
}
