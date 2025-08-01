#pragma once
#ifndef __TORPEDO_K__
#define __TORPEDO_K__

#include "TorpedoBase.h"

class TorpedoK final : public virtual TorpedoBase
{
public:
	TorpedoK(float speed = 0.0f, glm::vec2 direction = {0,0});
	~TorpedoK();

	// Life Cycle Methods
	virtual void draw() override;

private:
	void m_buildAnimations();

};

#endif /* defined (__PLAYER__) */