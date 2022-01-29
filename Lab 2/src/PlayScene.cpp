#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"

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

	if (m_bDebugView)
	{
		Util::DrawCircle(m_pTarget->getTransform()->position, m_pTarget->getWidth() * 0.5f);

		if (m_pStarship->isEnabled())
		{
			Util::DrawCircle(m_pStarship->getTransform()->position, Util::max(m_pStarship->getWidth() * 0.5f, m_pStarship->getHeight() * 0.5f));
		}
	}

	SDL_SetRenderDrawColor(Renderer::Instance().getRenderer(), 255, 255, 255, 255);
}

void PlayScene::update()
{
	updateDisplayList();
	CollisionManager::squaredRadiusCheck(m_pStarship, m_pTarget);
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
	m_bDebugView = false;

	m_pTarget = new Target();
	addChild(m_pTarget);

	m_pStarship = new Starship();
	m_pStarship->setCurrentHeading(0.0);
	m_pStarship->setTargetPosition(m_pTarget->getTransform()->position);
	m_pStarship->getRigidBody()->acceleration = m_pStarship->getCurrentDirection() * m_pStarship->getAccelerationRate();
	m_pStarship->setEnabled(false);
	addChild(m_pStarship);

	ImGuiWindowFrame::Instance().setGUIFunction(std::bind(&PlayScene::GUI_Function, this));
}

void PlayScene::GUI_Function()
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("Debug Window for Lab 2", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove);

	ImGui::Separator();
	
	static bool toggleDebug = false;
	if (ImGui::Checkbox("Toggle Debug", &toggleDebug))
	{
		m_bDebugView = toggleDebug;
	}
	
	ImGui::Separator();

	static float position[2] = {m_pTarget->getTransform()->position.x, m_pTarget->getTransform()->position.y};
	if(ImGui::SliderFloat2("Target Position", position, 0.0f, 800.0f))
	{
		m_pTarget->getTransform()->position = glm::vec2(position[0], position[1]);
		m_pStarship->setTargetPosition(m_pTarget->getTransform()->position);
	}
	
	ImGui::Separator();
	//starship properties

	static bool toggleSeek = false;
	if (ImGui::Checkbox("Toggle Seek", &toggleSeek))
	{
		m_pStarship->setEnabled(toggleSeek);
	}
	
	static float speed = m_pStarship->getMaxSpeed();
	if (ImGui::SliderFloat("Max Speed", &speed, 0.0f, 100.0f))
	{
		m_pStarship->setMaxSpeed(speed);
	}

	static float acceleration_rate = m_pStarship->getAccelerationRate();
	if (ImGui::SliderFloat("Acceleration", &acceleration_rate, 0.0f, 50.0f))
	{
		m_pStarship->setAccelerationRate(acceleration_rate);
		m_pStarship->getRigidBody()->acceleration = m_pStarship->getCurrentDirection() * m_pStarship->getAccelerationRate();
	}

	static float turn_rate = m_pStarship->getTurnRate();
	if (ImGui::SliderFloat("Turn Rate", &turn_rate, 0.0f, 20.0f))
	{
		m_pStarship->setTurnRate(turn_rate);
	}

	if (ImGui::Button("Reset"))
	{
		//reset positions
		m_pStarship->getTransform()->position = glm::vec2(100.0f, 400.0f);
		m_pTarget->getTransform()->position = glm::vec2(500.0f, 100.0f);
		//reset movement
		m_pStarship->setCurrentHeading(0.0);
		m_pStarship->getRigidBody()->velocity = glm::vec2(0, 0);
		m_pStarship->getRigidBody()->acceleration = m_pStarship->getCurrentDirection() * m_pStarship->getAccelerationRate();
		//reset target position
		m_pStarship->setTargetPosition(m_pTarget->getTransform()->position);
	}

	ImGui::End();
}
