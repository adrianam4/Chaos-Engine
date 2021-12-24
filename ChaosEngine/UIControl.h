#include <string>
#include "SDL/include/SDL.h"

enum class UIControlType
{
	BUTTON,
	CHECKBOX,
	SLIDER,
	INPUTBOX,
};

enum class UIControlState
{
	DISABLED,
	NORMAL,
	FOCUSED,
	PRESSED,
	SELECTED
};

class UIControl
{
public:

	UIControl(UIControlType type, int id) : type(type), id(id), state(UIControlState::NORMAL)
	{

	}

	UIControl(UIControlType type, SDL_Rect bounds, const char* text) :
		type(type),
		state(UIControlState::NORMAL),
		bounds(bounds),
		text(text)
	{
		color.r = 255; color.g = 255; color.b = 255;
		texture = NULL;

	}

	virtual ~UIControl()
	{
		text.clear();
	}

	virtual bool Update(float dt)
	{
		return true;
	}

	virtual bool Draw()
	{
		return true;
	}

	void SetTexture(SDL_Texture* tex)
	{
		texture = tex;
		section = { 0, 0, 0, 0 };
	}

	/*void SetObserver(Module* module)
	{
		observer = module;
	}

	void NotifyObserver()
	{
		observer->OnUIMouseClickEvent(this);
	}*/

public:

	int id;

	UIControlType type;
	UIControlState state;

	std::string text;
	SDL_Rect bounds;
	SDL_Color color;

	SDL_Texture* texture;
	SDL_Rect section;

	SDL_Texture* textureButtons;
	SDL_Texture* textureSliders;

	//Module* observer;

	bool pendingToDelete;
};