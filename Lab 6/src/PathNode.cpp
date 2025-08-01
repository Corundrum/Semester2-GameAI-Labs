#include "PathNode.h"
#include "Util.h"

PathNode::PathNode()
{
	setWidth(10);
	setHeight(10);
	getTransform()->position = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(PATH_NODE);

	setCurrentHeading(0.0f); // current facing angle
	setCurrentDirection(glm::vec2(1.0f, 0.0f)); // facing right

	setLOSDistance(1000.0f);
	setLOSColour(glm::vec4(0, 0.5, 1, 1));
}

PathNode::~PathNode()
= default;

void PathNode::draw()
{
	// draw the node

	if (hasLOS())
	{
		Util::DrawFilledRect(getTransform()->position - glm::vec2(5, 5), getWidth(), getHeight(), glm::vec4(0, 0.75, 0, 1));
	}
	else
	{
		Util::DrawRect(getTransform()->position - glm::vec2(5, 5), getWidth(), getHeight(), getLOSColour());
	}
}

void PathNode::update()
{
}

void PathNode::clean()
{
}

