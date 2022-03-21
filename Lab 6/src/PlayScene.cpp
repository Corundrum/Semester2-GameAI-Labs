#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"
#include "Config.h"
#include <fstream>

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"
#include "Util.h"

PlayScene::PlayScene()
{
	PlayScene::start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
	drawDisplayList();
	
	SDL_SetRenderDrawColor(Renderer::Instance().getRenderer(), 255, 255, 255, 255);
}

void PlayScene::update()
{
	updateDisplayList();
	m_checkAgentLOS(m_pSpaceShip, m_pTarget);
	switch (m_LOSMode)
	{
	case 0:
		m_checkAllNodesWithTarget(m_pTarget);
		break;
	case 1:
		m_checkAllNodesWithTarget(m_pSpaceShip);
		break;
	case 2:
		m_checkAllNodesWithBoth();
		break;
	case 3:
		m_connectTargets();
		break;
	}

	if (drawline)
	{
		Util::DrawLine(m_pSpaceShip->getTransform()->position, MiddleNode->getTransform()->position);
		Util::DrawLine(m_pTarget->getTransform()->position, MiddleNode->getTransform()->position, glm::vec4(0, 0, 1, 1));
	}
}

void PlayScene::clean()
{
	removeAllChildren();
}

void PlayScene::handleEvents()
{
	EventManager::Instance().update();

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance().quit();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance().changeSceneState(START_SCENE);
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_2))
	{
		TheGame::Instance().changeSceneState(END_SCENE);
	}
}

void PlayScene::start()
{

	// Set GUI Title
	m_guiTitle = "Play Scene";

	std::ifstream inFile("../Assets/data/obstacles.txt");
	while (!inFile.eof())
	{
		Obstacle* obstacle = new Obstacle();
		float x, y, w, h;
		inFile >> x >> y >> w >> h;
		obstacle->getTransform()->position = glm::vec2(x, y);
		obstacle->setWidth(w);
		obstacle->setHeight(h);
		addChild(obstacle);
		m_pObstacles.push_back(obstacle);
	}
	inFile.close();

	m_pTarget = new Target();
	m_pTarget->getTransform()->position = glm::vec2(600.0f, 300.0f);
	addChild(m_pTarget);

	m_pSpaceShip = new SpaceShip();
	m_pSpaceShip->getTransform()->position = glm::vec2(150.0f, 300.0f);
	addChild(m_pSpaceShip, 3);

	//set up a few fields
	m_LOSMode = 0;
	m_obstacleBuffer = 0;
	m_pathNodeLOSDistance = 1000;
	m_setPathNodeLOSDistance(m_pathNodeLOSDistance);

	//setup the grid
	m_isGridEnabled = false;
	m_buildGrid();
	m_toggleGrid(m_isGridEnabled);

	SoundManager::Instance().load("../Assets/audio/yay.ogg", "yay", SOUND_SFX);
	SoundManager::Instance().load("../Assets/audio/thunder.ogg", "boom", SOUND_SFX);

	ImGuiWindowFrame::Instance().setGUIFunction(std::bind(&PlayScene::GUI_Function, this));
}

void PlayScene::m_buildGrid()
{
	const auto tile_size = Config::TILE_SIZE;
	auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);

	//clear the nodes that exist 
	m_clearNodes();

	//add pathnodes to grid
	for (int row = 0; row < Config::ROW_NUM; ++row)
	{
		for (int col = 0; col < Config::COL_NUM; ++col)
		{
			PathNode* path_node = new PathNode();
			path_node->getTransform()->position = glm::vec2((col * tile_size) + offset.x, (row * tile_size) + offset.y);
			bool keepNode = true;
			for (auto obstacle : m_pObstacles)
			{
				if (CollisionManager::AABBCheckWithBuffer(path_node, obstacle, m_obstacleBuffer))
				{
					keepNode = false; // we have collision between node and an obstacle
				}
			}
			if (keepNode)
			{
				addChild(path_node);
				m_pGrid.push_back(path_node);
			}
			else
			{
				delete path_node;
			}
		}
	}

	// set grid visibility
	m_toggleGrid(m_isGridEnabled);

}

void PlayScene::m_toggleGrid(const bool state)
{
	for (auto path_node : m_pGrid)
	{
		path_node->setVisible(state);
	}
}

bool PlayScene::checkPathNodeLOS(PathNode* path_node, DisplayObject* target_object)
{
	auto targetDirection = target_object->getTransform()->position - path_node->getTransform()->position;
	auto normalizedDirection = Util::normalize(targetDirection);
	path_node->setCurrentDirection(normalizedDirection);


	return m_checkAgentLOS(path_node, target_object);
}

void PlayScene::m_checkAllNodesWithTarget(DisplayObject* target_object)
{
	for (auto path_node : m_pGrid)
	{
		checkPathNodeLOS(path_node, target_object);
	}
}

void PlayScene::m_checkAllNodesWithBoth()
{
	for (auto path_node : m_pGrid)
	{
		bool LOSWithSpaceShip = checkPathNodeLOS(path_node, m_pSpaceShip);
		bool LOSWithTarget = checkPathNodeLOS(path_node, m_pTarget);
		path_node->setHasLOS(LOSWithSpaceShip && LOSWithTarget ? true : false);
	}
}

void PlayScene::m_connectTargets()
{
	for (auto path_node : m_pGrid)
	{
		bool LOSWithSpaceShip = checkPathNodeLOS(path_node, m_pSpaceShip);
		bool LOSWithTarget = checkPathNodeLOS(path_node, m_pTarget);

		if ((LOSWithSpaceShip || LOSWithTarget))
		{
			path_node->setHasLOS(true);
		}
		else
		{
			path_node->setHasLOS(false);
		}

		path_node->setHasLOS(LOSWithSpaceShip || LOSWithTarget ? true : false);
	
		
	}
}

bool PlayScene::m_checkAgentLOS(Agent* agent, DisplayObject* target_object)
{
	bool hasLOS = false;
	agent->setHasLOS(hasLOS);

	auto AgentToTargetDistance = Util::getClosestEdge(agent->getTransform()->position, target_object);
	if (AgentToTargetDistance <= agent->getLOSDistance())
	{
		std::vector<DisplayObject*> contactList;

		for (auto object : getDisplayList())
		{
			auto AgentToObjectDistance = Util::getClosestEdge(agent->getTransform()->position, object);

			if (AgentToObjectDistance > AgentToTargetDistance) { continue; }
			
			if ((object->getType() != AGENT) && (object->getType() != PATH_NODE) && (object->getType() != TARGET))
			{	
				contactList.push_back(object);
			}
		}
		const glm::vec2 agentEndPoint = agent->getTransform()->position + agent->getCurrentDirection() * agent->getLOSDistance();
		hasLOS = CollisionManager::LOSCheck(agent, agentEndPoint, contactList, target_object);
		agent->setHasLOS(hasLOS);
	}
	return hasLOS;
}

void PlayScene::m_clearNodes()
{
	m_pGrid.clear();
	for (auto object : getDisplayList())
	{
		if (object->getType() == PATH_NODE)
		{
			removeChild(object);
		}
	}
}

void PlayScene::m_setPathNodeLOSDistance(int dist)
{
	for (auto path_node : m_pGrid)
	{
		path_node->setLOSDistance((float)dist);
	}
}

void PlayScene::findPath(Agent* agent, DisplayObject* target_object)
{
	std::vector<PathNode*> pathlist;
	

	for (auto path_node : m_pGrid)
	{
		bool LOSWithSpaceShip = checkPathNodeLOS(path_node, m_pSpaceShip);
		bool LOSWithTarget = checkPathNodeLOS(path_node, m_pTarget);

		if (LOSWithSpaceShip || LOSWithTarget)
		{
			pathlist.push_back(path_node);
		}

	}
	for (auto path : pathlist)
	{
		auto agentDistance = Util::distance(agent->getTransform()->position, path->getTransform()->position);
		auto targetDistance = Util::distance(target_object->getTransform()->position, path->getTransform()->position);
		
		//auto MiddleDistance = (Util::distance(agent->getTransform()->position, target_object->getTransform()->position) / 2);
		double lowestDistance = 10000;
		

		if ((MiddleNode == nullptr) || ((agentDistance + targetDistance) / 2) <= lowestDistance)
		{
			lowestDistance = ((agentDistance + targetDistance) / 2);
			MiddleNode = path;
		}

		std::cout << lowestDistance << std::endl;
		
	}
	std::cout << std::endl << std::endl << std::endl << MiddleNode->getTransform()->position.x << ", " << MiddleNode->getTransform()->position.y << std::endl;
	drawline = true;
}

void PlayScene::GUI_Function()
{
	auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);
	// Always open with a NewFrame
	ImGui::NewFrame();

	ImGui::Begin("Lab 6 Debug Properties", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove);

	ImGui::Separator();


	if (ImGui::Checkbox("Toggle Grid", &m_isGridEnabled))
	{
		m_toggleGrid(m_isGridEnabled);
	}

	ImGui::Separator();

	if (ImGui::Button("Node LOS to Target", { 300, 20 } ))
	{
		m_LOSMode = 0;
	}
	if (m_LOSMode == 0)
	{
		ImGui::SameLine();
		ImGui::Text("<Active>");
	}

	if (ImGui::Button("Node LOS to SpaceShip", { 300, 20 } ))
	{
		m_LOSMode = 1;
	}
	if (m_LOSMode == 1)
	{
		ImGui::SameLine();
		ImGui::Text("<Active>");
	}

	if (ImGui::Button("Node LOS to Both", { 300, 20 } ))
	{
		m_LOSMode = 2;
	}
	if (m_LOSMode == 2)
	{
		ImGui::SameLine();
		ImGui::Text("<Active>");
	}

	if (ImGui::Button("Connect Nodes", { 300, 20 }))
	{
		m_LOSMode = 3;
	}
	if (m_LOSMode == 3)
	{
		ImGui::SameLine();
		ImGui::Text("<Active>");
	}

	if (ImGui::Button("Find Path"))
	{
		findPath(m_pSpaceShip, m_pTarget);
	}

	ImGui::Separator();

	static int shipPosition[] = { m_pSpaceShip->getTransform()->position.x, m_pSpaceShip->getTransform()->position.y };
	if (ImGui::SliderInt2("Ship Position", shipPosition, 0, 800))
	{
		m_pSpaceShip->getTransform()->position.x = shipPosition[0];
		m_pSpaceShip->getTransform()->position.y = shipPosition[1];
	}

	//allow ship rotation
	static int angle;
	if (ImGui::SliderInt("Ship Direction", &angle, -180, 180))
	{
		m_pSpaceShip->setCurrentHeading(angle);
	}

	ImGui::Separator();

	static int targetPosition[] = { m_pTarget->getTransform()->position.x, m_pTarget->getTransform()->position.y};
	if (ImGui::SliderInt2("Target Position", targetPosition, 0, 800))
	{
		m_pTarget->getTransform()->position.x = targetPosition[0];
		m_pTarget->getTransform()->position.y = targetPosition[1];
	}

	ImGui::Separator();

	for (unsigned i = 0; i < m_pObstacles.size(); ++i)
	{
		int obsPosition[] = { m_pObstacles[i]->getTransform()->position.x, m_pObstacles[i]->getTransform()->position.y };
		std::string label = "Obstacle" + std::to_string(i + 1) + " Position";
		if (ImGui::SliderInt2(label.c_str(), obsPosition, 0, 800))
		{
			m_pObstacles[i]->getTransform()->position.x = obsPosition[0];
			m_pObstacles[i]->getTransform()->position.y = obsPosition[1];
		}
	}

	ImGui::Separator();

	if (ImGui::SliderInt("Obstacle Buffer", &m_obstacleBuffer, 0, 100))
	{
		m_buildGrid();
	}



	ImGui::End();
}