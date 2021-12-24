#include "SliderComponent.h"
#include "Application.h"

SliderComponent::SliderComponent(int id, SDL_Rect bounds, const char* text, SDL_Texture* textureButton, SDL_Texture* textureSlider) : UIControl(UIControlType::SLIDER, id)
{
	this->bounds = bounds;
	this->text = text;
	minValue = 0;
	maxValue = 100;
	textureButtons = textureButton;
	textureSliders = textureSlider;
	drawRect = false;
	pendingToDelete = false;
}

SliderComponent::~SliderComponent()
{
}

bool SliderComponent::Update(float dt)
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
		else state = UIControlState::NORMAL;

		if (value > maxValue)
		{
			value = maxValue;
		}

		else if (value < minValue)
		{
			value = minValue;
		}
	}
	return false;
}

bool SliderComponent::Draw()
{
	SDL_Rect rect = { 3486,3838,359,57 };
	SDL_Rect rect2 = { 3450,3955, 3.41 * value,46 };
	SDL_Rect rect3;

	if (id == 1)
	{
		rect3 = { 1074,921, 91,96 };
	}

	if (id == 2)
	{
		rect3 = { 1074,1029, 91,96 };
	}

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