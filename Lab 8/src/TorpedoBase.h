#pragma once
#ifndef __TORPEDO_BASE__
#define __TORPEDO_BASE__

#include "Sprite.h"
#include "TorpedoAnimationState.h"

class TorpedoBase : public virtual Sprite
{
public:
	TorpedoBase(float speed = 0.0f, glm::vec2 direction = { 0,0 });
	~TorpedoBase();

	// Life Cycle Methods
	virtual void draw() override = 0;
	virtual void update() override;
	virtual void clean() override;

	// setters
	void setAnimationState(TorpedoAnimationState new_state);
	TorpedoAnimationState getAnimationState() { return m_currentAnimationState; }

private:
	virtual void m_buildAnimations() = 0;

	TorpedoAnimationState m_currentAnimationState;

	glm::vec2 m_direction;
};


#endif // !__TORPEDO_BASE__

