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
	SDL_SetRenderDrawColor(Renderer::Instance().getRenderer(), 255, 255, 255, 255);
}

void PlayScene::update()
{
	updateDisplayList();

	//m_pStarship->setDesiredVelocity(m_pTarget->getTransform()->position);
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

	m_pTarget = new Target();
	addChild(m_pTarget);

	m_pStarship = new Starship();
	m_pStarship->setCurrentDirection(glm::vec2(1.0f, 0.0f));
	m_pStarship->setTargetPosition(m_pTarget->getTransform()->position);
	m_pStarship->setEnabled(false);
	addChild(m_pStarship);

	ImGuiWindowFrame::Instance().setGUIFunction(std::bind(&PlayScene::GUI_Function, this));
}

void PlayScene::GUI_Function() const
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("Debug Window for Lab 2", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove);

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
	}

	static float turn_rate = m_pStarship->getTurnRate();
	if (ImGui::SliderFloat("Turn Rate", &turn_rate, 0.0f, 20.0f))
	{
		m_pStarship->setTurnRate(turn_rate);
	}

	ImGui::End();
}
