#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Heuristic.h"
#include "Scene.h"
#include "Target.h"
#include "PathNode.h"
#include "Obstacle.h"
// New Lab 7:
#include "Background.h"
#include "CloseCombatEnemy.h"
#include "RangedCombatEnemy.h"
#include "TorpedoF.h"
#include "TorpedoK.h"

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

	//new for lab 8
	void SpawnEnemyTorpedo();

private:
	// IMGUI Function
	void GUI_Function();
	std::string m_guiTitle;
	glm::vec2 m_mousePosition;
	bool m_isGridEnabled;
	// New for Lab 7
	int shipPosition[2];
	int angle;

	// Game Objects
	Background* m_pBG;
	Target* m_pTarget;
	CloseCombatEnemy* m_pCloseSpaceShip;
	RangedCombatEnemy* m_pSpaceShip;

	std::vector<Obstacle*> m_pObstacles;
	std::vector<TorpedoBase*> m_torpedoes;
	//std::vector<TorpedoF*> m_pTorpedoes;
	//std::vector<TorpedoK*> m_pTorpedoesK;

	// PathNode Objects and Functions
	std::vector<PathNode*> m_pGrid;
	void m_buildGrid();
	void m_toggleGrid(bool state);
	bool m_checkAgentLOS(Agent* agent, DisplayObject* target_object);
	bool m_checkPathNodeLOS(PathNode* path_node, DisplayObject* target_object);
	void m_checkAllNodesWithTarget(DisplayObject* target_object);
	void m_checkAllNodesWithBoth();
	void m_clearNodes();
	void m_setPathNodeLOSDistance(int dist);
	PathNode* m_walkOnShortestPath();

	float timer = 0;

	int m_LOSMode; // 0 = nodes visible to target, 1 = nodes visible to player, 2 = nodes visible to both
	int m_obstacleBuffer;
	int m_pathNodeLOSDistance;
};

#endif /* defined (__PLAY_SCENE__) */