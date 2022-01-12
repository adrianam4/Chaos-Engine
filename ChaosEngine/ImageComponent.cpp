#include "Application.h"
#include "SDL.h"
#include "ImageComponent.h"

ImageComponent::ImageComponent(int id, std::string text)
{
	name = "Image Component";
	type = ComponentType::UI_IMAGE;
	this->text = "Image Component";
	state = State::NORMAL;
	color = White;
}

ImageComponent::~ImageComponent()
{
	text.clear();
}

void ImageComponent::Update()
{
}

void ImageComponent::Draw()
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

void ImageComponent::OnEditor(int i)
{
	// Manage if colors are being edited or not
	static bool normalEditable = false;

	ImGui::Checkbox("Active", &active);

	ImGui::Text("Image Color"); ImGui::SameLine();
	if (ImGui::ColorButton("Image Color", ImVec4(color.r, color.g, color.b, color.a)))
		normalEditable = !normalEditable;

	if (normalEditable)
	{
		ImGui::ColorPicker3("Image Color", &color);
	}
}
