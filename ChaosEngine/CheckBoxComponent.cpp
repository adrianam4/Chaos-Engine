#include "CheckboxComponent.h"
#include "Application.h"

CheckboxComponent::CheckboxComponent(int id, SDL_Rect bounds, const char* text, SDL_Texture* textureButton) : UIControl(UIControlType::CHECKBOX, id)
{
	this->bounds = bounds;
	this->text = text;
	textureButtons = textureButton;
	drawRectangle = false;
	pendingToDelete = false;
}

CheckboxComponent::~CheckboxComponent()
{
}

bool CheckboxComponent::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
	{
		if (drawRectangle == false)
		{
			drawRectangle = true;
		}
		else
		{
			drawRectangle = false;
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
			/*if (state != UIControlState::FOCUSED && state != UIControlState::PRESSED)
			{
				App->audio->PlayFx(focusedFX);
			}*/
			state = UIControlState::FOCUSED;

			if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
			{
				state = UIControlState::PRESSED;
			}

			if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP)
			{
				checked = !checked;
				//NotifyObserver();
			}
		}
		else state = UIControlState::NORMAL;
	}



	return false;
}

bool CheckboxComponent::Draw()
{
	/*SDL_Rect rect;
	if (id == 8)
	{
		if (App->win->fullScreenWindow == true)
		{
			checked = true;
		}
		else
		{
			checked = false;
		}
		rect = { 297,1360,91,96 };
	}
	if (id == 9)
	{
		if (App->vsync == true)
		{
			checked = true;
		}
		else
		{
			checked = false;
		}
		rect = { 297,1360,91,96 };
	}*/
	// Draw the right button depending on state
	switch (state)
	{
	case UIControlState::DISABLED:
	{
		/*if (checked) 	app->render->DrawTexture(textureButtons, bounds.x, bounds.y, &rect8);
		else 	app->render->DrawTexture(textureButtons, bounds.x, bounds.y, &rect4);
		app->render->DrawTexture(textureButtons, bounds.x + 95, bounds.y + 15, &rect9);
		if (drawRectangle == true) app->render->DrawRectangle(bounds, 0, 255, 0, 255);*/
	} break;
	case UIControlState::NORMAL:
	{
		/*if (checked) 	app->render->DrawTexture(textureButtons, bounds.x, bounds.y, &rect5);
		else 	app->render->DrawTexture(textureButtons, bounds.x, bounds.y, &rect);
		app->render->DrawTexture(textureButtons, bounds.x + 95, bounds.y + 15, &rect9);
		if (drawRectangle == true) app->render->DrawRectangle(bounds, 0, 255, 0, 255);*/
	} break;
	case UIControlState::FOCUSED:
	{
		/*if (checked) 	app->render->DrawTexture(textureButtons, bounds.x, bounds.y, &rect6);
		else 	app->render->DrawTexture(textureButtons, bounds.x, bounds.y, &rect2);
		app->render->DrawTexture(textureButtons, bounds.x + 95, bounds.y + 15, &rect9);
		if (drawRectangle == true) app->render->DrawRectangle(bounds, 255, 255, 0, 255);*/
	} break;
	case UIControlState::PRESSED:
	{
		/*if (checked) 	app->render->DrawTexture(textureButtons, bounds.x, bounds.y, &rect7);
		else 	app->render->DrawTexture(textureButtons, bounds.x, bounds.y, &rect3);
		app->render->DrawTexture(textureButtons, bounds.x + 95, bounds.y + 15, &rect9);
		if (drawRectangle == true) app->render->DrawRectangle(bounds, 255, 0, 0, 255);*/
	} break;
	default:
		break;
	}

	return false;
}