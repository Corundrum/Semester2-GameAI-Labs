#pragma once
#ifndef __PATH_NODE__
#define __PATH_NODE__

#include "Agent.h"

class PathNode : public Agent
{
public:
	//constructor
	PathNode();

	//destructor
	~PathNode();

	//lifecycle function
	void draw() override;
	void update() override;
	void clean() override;

private:

};

#endif // !__PATH_NODE__
