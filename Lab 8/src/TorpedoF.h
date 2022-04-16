#pragma once
#ifndef __TORPEDO__
#define __TORPEDO__

#include "TorpedoBase.h"

class TorpedoF final : public virtual TorpedoBase
{
public:
	TorpedoF(float speed = 0.0f, glm::vec2 direction = {0,0});
	~TorpedoF();

	// Life Cycle Methods
	virtual void draw() override;



private:
	void m_buildAnimations();

};

#endif /* defined (__PLAYER__) */