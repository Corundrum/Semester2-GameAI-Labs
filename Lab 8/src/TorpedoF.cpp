#include "TorpedoF.h"
#include "TextureManager.h"

TorpedoF::TorpedoF(float speed, glm::vec2 direction) : TorpedoBase(speed, direction)
{
	TextureManager::Instance().loadSpriteSheet(
		"../Assets/sprites/torpedo.txt",
		"../Assets/sprites/torpedo.png", 
		"torpedosheet");
	setSpriteSheet(TextureManager::Instance().getSpriteSheet("torpedosheet"));

	m_buildAnimations();
}

TorpedoF::~TorpedoF()
= default;

void TorpedoF::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the player according to animation state
	switch(getAnimationState())
	{
	case FIRED:
		TextureManager::Instance().playAnimation("torpedosheet", getAnimation("fired"),
			x, y, 5.0f, 0, 255, true);
		break;
	default:
		break;
	}
	
}

void TorpedoF::m_buildAnimations()
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
