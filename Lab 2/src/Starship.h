#pragma once
#ifndef __STARSHIP__
#define __STARSHIP__
#include "DisplayObject.h"

class Starship final : public DisplayObject
{
public:
	Starship();
	~Starship();

	// Inherited via GameObject
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

private:

};


#endif // !1
