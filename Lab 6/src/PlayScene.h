#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Heuristic.h"
#include "Scene.h"
#include "Target.h"
#include "SpaceShip.h"
#include "Obstacle.h"
#include "PathNode.h"


class PlayScene : public Scene
{
public:
	PlayScene();
	~PlayScene();

	// Scene LifeCycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;
private:
	// IMGUI Function
	void GUI_Function();
	std::string m_guiTitle;
	glm::vec2 m_mousePosition;
	bool m_isGridEnabled;

	// Game Objects
	Target* m_pTarget;
	SpaceShip* m_pSpaceShip;
	std::vector<Obstacle*> m_pObstacles;

	PathNode* MiddleNode;

	// pathnode objects objects and functions
	std::vector<PathNode*> m_pGrid;
	void m_buildGrid();
	void m_toggleGrid(bool state);
	bool m_checkAgentLOS(Agent* agent, DisplayObject* target_object);
	bool checkPathNodeLOS(PathNode* path_node, DisplayObject* target_object);
	void m_checkAllNodesWithTarget(DisplayObject* target_object);
	void m_checkAllNodesWithBoth();
	void m_connectTargets();
	void m_clearNodes();
	void m_setPathNodeLOSDistance(int dist);
	void findPath(Agent* agent, DisplayObject* target_object);

	bool drawline = false;

	int m_LOSMode;
	int m_obstacleBuffer;
	int m_pathNodeLOSDistance;
};

#endif /* defined (__PLAY_SCENE__) */