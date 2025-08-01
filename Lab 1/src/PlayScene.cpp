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
	if (Util::distance(m_pPlaneSprite->getTransform()->position, m_pPlayer->getTransform()->position) < 65)
	{
		TheGame::Instance().changeSceneState(END_SCENE);
	}
}

void PlayScene::clean()
{
	removeAllChildren();
}

void PlayScene::handleEvents()
{
	EventManager::Instance().update();

	// handle player movement with GameController
	//if (SDL_NumJoysticks() > 0)
	//{
	//	if (EventManager::Instance().getGameController(0) != nullptr)
	//	{
	//		const auto deadZone = 10000;
	//		if (EventManager::Instance().getGameController(0)->LEFT_STICK_X > deadZone)
	//		{
	//			m_pPlayer->setAnimationState(PLAYER_RUN_RIGHT);
	//			m_playerFacingRight = true;
	//		}
	//		else if (EventManager::Instance().getGameController(0)->LEFT_STICK_X < -deadZone)
	//		{
	//			m_pPlayer->setAnimationState(PLAYER_RUN_LEFT);
	//			m_playerFacingRight = false;
	//		}
	//		else
	//		{
	//			if (m_playerFacingRight)
	//			{
	//				m_pPlayer->setAnimationState(PLAYER_IDLE_RIGHT);
	//			}
	//			else
	//			{
	//				m_pPlayer->setAnimationState(PLAYER_IDLE_LEFT);
	//			}
	//		}
	//	}
	//}


	// handle player movement if no Game Controllers found
	/*if (SDL_NumJoysticks() < 1)
	{*/
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_A))
		{
			m_pPlayer->setAnimationState(PLAYER_RUN_LEFT);
			m_playerFacingRight = false;
			const auto current_position = m_pPlayer->getTransform()->position;
			m_pPlayer->getTransform()->position = current_position + glm::vec2(-5.0f, 0.0f);
		}
		else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_D))
		{
			m_pPlayer->setAnimationState(PLAYER_RUN_RIGHT);
			m_playerFacingRight = true;
			const auto current_position = m_pPlayer->getTransform()->position;
			m_pPlayer->getTransform()->position = current_position + glm::vec2(5.0f, 0.0f);
		}
		else
		{
			if (m_playerFacingRight)
			{
				m_pPlayer->setAnimationState(PLAYER_IDLE_RIGHT);
			}
			else
			{
				m_pPlayer->setAnimationState(PLAYER_IDLE_LEFT);
			}
		}
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_W))
		{
			const auto current_position = m_pPlayer->getTransform()->position;
			m_pPlayer->getTransform()->position = current_position + glm::vec2(0.0f, -5.0f);
		}
		else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_S))
		{
			const auto current_position = m_pPlayer->getTransform()->position;
			m_pPlayer->getTransform()->position = current_position + glm::vec2(0.0f, 5.0f);
		}
	/*}*/
	

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
	
	// Plane Sprite
	m_pPlaneSprite = new Plane();
	m_pPlaneSprite->getTransform()->position = glm::vec2(400.0f, 100.0f);
	addChild(m_pPlaneSprite);

	// Player Sprite
	m_pPlayer = new Player();
	m_pPlayer->getTransform()->position = glm::vec2(400.0f, 500.0f);
	addChild(m_pPlayer);
	m_playerFacingRight = true;

	// Back Button
	//m_pBackButton = new Button("../Assets/textures/backButton.png", "backButton", BACK_BUTTON);
	//m_pBackButton->getTransform()->position = glm::vec2(300.0f, 400.0f);
	//m_pBackButton->addEventListener(CLICK, [&]()-> void
	//{
	//	m_pBackButton->setActive(false);
	//	TheGame::Instance().changeSceneState(START_SCENE);
	//});

	//m_pBackButton->addEventListener(MOUSE_OVER, [&]()->void
	//{
	//	m_pBackButton->setAlpha(128);
	//});

	//m_pBackButton->addEventListener(MOUSE_OUT, [&]()->void
	//{
	//	m_pBackButton->setAlpha(255);
	//});
	//addChild(m_pBackButton);

	//// Next Button
	//m_pNextButton = new Button("../Assets/textures/nextButton.png", "nextButton", NEXT_BUTTON);
	//m_pNextButton->getTransform()->position = glm::vec2(500.0f, 400.0f);
	//m_pNextButton->addEventListener(CLICK, [&]()-> void
	//{
	//	m_pNextButton->setActive(false);
	//	TheGame::Instance().changeSceneState(END_SCENE);
	//});

	//m_pNextButton->addEventListener(MOUSE_OVER, [&]()->void
	//{
	//	m_pNextButton->setAlpha(128);
	//});

	//m_pNextButton->addEventListener(MOUSE_OUT, [&]()->void
	//{
	//	m_pNextButton->setAlpha(255);
	//});

	//addChild(m_pNextButton);

	///* Instructions Label */
	//m_pInstructionsLabel = new Label("Press the backtick (`) character to toggle Debug View", "Consolas");
	//m_pInstructionsLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.5f, 500.0f);

	//addChild(m_pInstructionsLabel);

	ImGuiWindowFrame::Instance().setGUIFunction(std::bind(&PlayScene::GUI_Function, this));
}

void PlayScene::GUI_Function() const
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("Your Window Title Goes Here", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove);

	if(ImGui::Button("My Button"))
	{
		std::cout << "My Button Pressed" << std::endl;
	}

	ImGui::Separator();

	static float float2[2] = {m_pPlaneSprite->getTransform()->position.x, m_pPlaneSprite->getTransform()->position.y};
	if(ImGui::SliderFloat2("Plane Position", float2, 0.0f, 800.0f))
	{
		m_pPlaneSprite->getTransform()->position = glm::vec2(float2[0], float2[1]);
	}
	
	ImGui::End();
}
