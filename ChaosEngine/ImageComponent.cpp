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

bool ImageComponent::Update(float dt)
{
	return false;
}

void ImageComponent::Draw()
{
}
