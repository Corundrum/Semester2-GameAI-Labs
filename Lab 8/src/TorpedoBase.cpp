#include "TorpedoBase.h"

TorpedoBase::TorpedoBase(float speed, glm::vec2 direction) : m_currentAnimationState(FIRED)
{
	setWidth(64);
	setHeight(64);

	setType(PROJECTILE);
	m_direction = { direction.x * speed, direction.y * speed };
}

TorpedoBase::~TorpedoBase()
= default;


void TorpedoBase::update()
{
	this->getTransform()->position += m_direction;
}

void TorpedoBase::clean(){}

void TorpedoBase::setAnimationState(TorpedoAnimationState new_state)
{
	m_currentAnimationState = new_state;
}
