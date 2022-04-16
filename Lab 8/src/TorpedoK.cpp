#include "TorpedoK.h"
#include "TextureManager.h"

TorpedoK::TorpedoK(float speed, glm::vec2 direction): TorpedoBase(speed, direction)
{
	TextureManager::Instance().loadSpriteSheet(
		"../Assets/sprites/torpedo.txt",
		"../Assets/sprites/torpedo_k.png", 
		"torpedo_k_sheet");

	setSpriteSheet(TextureManager::Instance().getSpriteSheet("torpedo_k_sheet"));

	m_buildAnimations();
}

TorpedoK::~TorpedoK()
= default;

void TorpedoK::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the player according to animation state
	switch(getAnimationState())
	{
	case FIRED:
		TextureManager::Instance().playAnimation("torpedo_k_sheet", getAnimation("fired"),
			x, y, 5.0f, 0, 255, true);
		break;
	default:
		break;
	}	
}

void TorpedoK::m_buildAnimations()
{
	Animation firedAnimation = Animation();

	firedAnimation.name = "fired";
	firedAnimation.frames.push_back(getSpriteSheet()->getFrame("fired1"));
	firedAnimation.frames.push_back(getSpriteSheet()->getFrame("fired2"));
	firedAnimation.frames.push_back(getSpriteSheet()->getFrame("fired3"));
	firedAnimation.frames.push_back(getSpriteSheet()->getFrame("fired4"));
	firedAnimation.frames.push_back(getSpriteSheet()->getFrame("fired5"));
	firedAnimation.frames.push_back(getSpriteSheet()->getFrame("fired6"));
	firedAnimation.frames.push_back(getSpriteSheet()->getFrame("fired7"));
	firedAnimation.frames.push_back(getSpriteSheet()->getFrame("fired8"));

	setAnimation(firedAnimation);
}
