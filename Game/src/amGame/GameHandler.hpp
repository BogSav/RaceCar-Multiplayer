#pragma once

#include "amConnectivity/Connection.hpp"
#include "amGame/Game.hpp"
#include "amUI/MainMenu.hpp"

#include <iostream>
#include <mutex>
#include <thread>

class GameHandler
{
public:
	GameHandler(std::string selfDirPath)
	{
		this->selfDirPath = selfDirPath;
		handle_entierty();
	}

private:
	void handle_entierty()
	{
		this->init_engine();
		this->handle_menu();
		this->handle_multiplayer_if_needed();
		this->handle_game();

		Engine::Exit();
	}

	void init_engine()
	{
		WindowProperties wp;
		wp.resolution = glm::ivec2(1280, 720);
		wp.vSync = true;
		wp.selfDir = selfDirPath;

		(void)Engine::Init(wp);
	}

	void handle_menu()
	{
		MainMenu* mainMenu = new MainMenu();
		mainMenu->Init();
		mainMenu->Run();
		mainMenu->UpdateGameSettings();
		delete mainMenu;
	}

	void handle_game()
	{
		Game* game = new Game();
		game->Init();
		game->Run();
		delete game;
	}

	void handle_multiplayer_if_needed()
	{
		if (Engine::GetGameSettings()->m_isMultiplayer)
		{
			Engine::GetConnection()->StartConnection();
			Engine::GetConnection()->PauseGame();
		}
	}

private:
	std::string selfDirPath;
};