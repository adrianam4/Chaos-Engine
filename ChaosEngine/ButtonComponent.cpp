#include "Application.h"
#include "SDL.h"
#include "ButtonComponent.h"

ButtonComponent::ButtonComponent(int id, SDL_Rect bounds, std::string text, SDL_Texture* textureButton)
{
	name = "Button Component";
	this->bounds = bounds;
	this->text = text;
	texture = textureButton;
	state = State::NORMAL;
}

ButtonComponent::~ButtonComponent()
{
	text.clear();
	delete texture;
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

	if (state != State::DISABLED)
	{
		int mouseX = App->input->GetMouseX();
		int mouseY = App->input->GetMouseY();

		// Check collision between mouse and button bounds
		if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) &&
			(mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)))
		{
			if (state != State::FOCUSED && state != State::PRESSED)
			{
				/*app->audio->PlayFx(focusedFX);*/
			}
			state = State::FOCUSED;

			if (App->input->GetKey(SDL_BUTTON_LEFT) == KEY_REPEAT)
			{
				state = State::PRESSED;
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

	return false;
}

void ButtonComponent::Draw()
{
	switch (state)
	{
	case State::DISABLED:
		/*App->render->DrawTexture(textureButtons, bounds.x, bounds.y, &rect);
		if (buttonsColliders == true)
		{
			app->render->DrawRectangle(bounds, 0, 255, 0, 255);
		}*/
		break;

	case State::NORMAL:
		/*App->render->DrawTexture(textureButtons, bounds.x, bounds.y, &rect2);
		if (buttonsColliders == true)
		{
			app->render->DrawRectangle(bounds, 0, 255, 0, 255);
		}*/
		break;

	case State::FOCUSED:
		/*App->render->DrawTexture(textureButtons, bounds.x, bounds.y, &rect3);
		if (buttonsColliders == true)
		{
			app->render->DrawRectangle(bounds, 255, 255, 0, 255);
		}*/
		break;

	case State::PRESSED:
		/*App->render->DrawTexture(textureButtons, bounds.x, bounds.y, &rect4);
		if (buttonsColliders == true)
		{
			app->render->DrawRectangle(bounds, 0, 255, 255, 255);
		}*/
		break;

	case State::SELECTED:
		/*App->render->DrawRectangle(bounds, 0, 255, 0, 255);*/
		break;

	default:
		break;
	}
}