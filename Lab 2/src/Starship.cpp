#include "Starship.h"
#include "TextureManager.h"


Starship::Starship()
{
	TextureManager::Instance().load("../Assets/textures/ncl.png","starship");

	const auto size = TextureManager::Instance().getTextureSize("starship");
	setWidth(size.x);
	setHeight(size.y);
	getTransform()->position = glm::vec2(600.0f, 500.0f);
	getRigidBody()->velocity = glm::vec2(0, 0);
	getRigidBody()->isColliding = false;

	setType(TARGET);
}

Starship::~Starship()
= default;

void Starship::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the target
	TextureManager::Instance().draw("starship", x, y, 0, 255, true);
}

void Starship::update()
{
	
}

void Starship::clean()
{
}
