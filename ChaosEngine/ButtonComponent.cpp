#include "ButtonComponent.h"
#include "Application.h"

ButtonComponent::ButtonComponent(int id, SDL_Rect bounds, const char* text, SDL_Texture* textureButton) : UIControl(UIControlType::BUTTON, id)
{

	this->bounds = bounds;
	this->text = text;
	buttonsColliders = false;
	textureButtons = textureButton;
	pendingToDelete = false;
}

ButtonComponent::~ButtonComponent()
{

}

bool ButtonComponent::Update(float dt)
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

	if (state != UIControlState::DISABLED)
	{
		int mouseX = App->input->GetMouseX();
		int mouseY = App->input->GetMouseY();

		// Check collision between mouse and button bounds
		if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) &&
			(mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)))
		{
			if (state != UIControlState::FOCUSED && state != UIControlState::PRESSED)
			{
				/*app->audio->PlayFx(focusedFX);*/
			}
			state = UIControlState::FOCUSED;

			if (App->input->GetKey(SDL_BUTTON_LEFT) == KEY_REPEAT)
			{
				state = UIControlState::PRESSED;
			}

			// If mouse button pressed -> Generate event!
			if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP)
			{
				/*App->audio->PlayFx(clickedFX);
				NotifyObserver();*/
			}
		}
		else state = UIControlState::NORMAL;
	}

	return false;
}

bool ButtonComponent::Draw()
{
	switch (state)
	{
	case UIControlState::DISABLED:
		/*App->render->DrawTexture(textureButtons, bounds.x, bounds.y, &rect);
		if (buttonsColliders == true)
		{
			app->render->DrawRectangle(bounds, 0, 255, 0, 255);
		}*/
		break;

	case UIControlState::NORMAL:
		/*App->render->DrawTexture(textureButtons, bounds.x, bounds.y, &rect2);
		if (buttonsColliders == true)
		{
			app->render->DrawRectangle(bounds, 0, 255, 0, 255);
		}*/
		break;

	case UIControlState::FOCUSED:
		/*App->render->DrawTexture(textureButtons, bounds.x, bounds.y, &rect3);
		if (buttonsColliders == true)
		{
			app->render->DrawRectangle(bounds, 255, 255, 0, 255);
		}*/
		break;

	case UIControlState::PRESSED:
		/*App->render->DrawTexture(textureButtons, bounds.x, bounds.y, &rect4);
		if (buttonsColliders == true)
		{
			app->render->DrawRectangle(bounds, 0, 255, 255, 255);
		}*/
		break;

	case UIControlState::SELECTED:
		/*App->render->DrawRectangle(bounds, 0, 255, 0, 255);*/
		break;

	default:
		break;
	}

	return false;
}