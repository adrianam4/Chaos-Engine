#include "InputBoxComponent.h"
#include "Application.h"

InputBoxComponent::InputBoxComponent(int id, SDL_Rect bounds, const char* text, SDL_Texture* textureButton) : UIControl(UIControlType::SLIDER, id)
{
	this->bounds = bounds;
	this->text = text;
	drawRect = false;
	pendingToDelete = false;
}

InputBoxComponent::~InputBoxComponent()
{
}

bool InputBoxComponent::Update(float dt)
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

	if (state != UIControlState::DISABLED)
	{
		int mouseX = App->input->GetMouseX();
		int mouseY = App->input->GetMouseY();

		// Check collision between mouse and button bounds
		if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) &&
			(mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)))
		{
			state = UIControlState::FOCUSED;
			if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
			{
				state = UIControlState::PRESSED;

				//NotifyObserver();
			}
		}
		else state = UIControlState::NORMAL;

	}
	return false;
}

bool InputBoxComponent::Draw()
{
	switch (state)
	{
	case UIControlState::DISABLED:
		/*app->render->DrawTexture(textureSliders, bounds.x, bounds.y, &rect);
		app->render->DrawTexture(textureSliders, bounds.x + 8, bounds.y + 6, &rect2);
		app->render->DrawTexture(textureButtons, bounds.x - 95, bounds.y, &rect3);
		if (drawRect == true) app->render->DrawRectangle(bounds, 0, 255, 0, 255);
		*/
		break;
	case UIControlState::NORMAL:
		/*app->render->DrawTexture(textureSliders, bounds.x, bounds.y, &rect);
		app->render->DrawTexture(textureSliders, bounds.x + 8, bounds.y + 6, &rect2);
		app->render->DrawTexture(textureButtons, bounds.x - 95, bounds.y, &rect3);
		if (drawRect == true) app->render->DrawRectangle(bounds, 0, 255, 0, 255);
		*/
		break;
	case UIControlState::FOCUSED:
		/*app->render->DrawTexture(textureSliders, bounds.x, bounds.y, &rect);
		app->render->DrawTexture(textureSliders, bounds.x + 8, bounds.y + 6, &rect2);
		app->render->DrawTexture(textureButtons, bounds.x - 95, bounds.y, &rect3);
		if (drawRect == true) app->render->DrawRectangle(bounds, 255, 255, 0, 255);
		*/
		break;
	case UIControlState::PRESSED:
		/*app->render->DrawTexture(textureSliders, bounds.x, bounds.y, &rect);
		app->render->DrawTexture(textureSliders, bounds.x + 8, bounds.y + 6, &rect2);
		app->render->DrawTexture(textureButtons, bounds.x - 95, bounds.y, &rect3);
		if (drawRect == true) app->render->DrawRectangle(bounds, 255, 0, 0, 255);
		*/
		break;
	default:
		break;
	}

	return false;
}