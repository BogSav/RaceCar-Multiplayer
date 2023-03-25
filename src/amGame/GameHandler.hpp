#pragma once

#include "amGame/Game.hpp"
#include "amGame/GameSettings.hpp"
#include "amUI/MainMenu.hpp"

#include <iostream>
#include <mutex>
#include <thread>

class GameHandler : public std::thread
{
public:
	GameHandler(
		std::mutex& mutex,
		std::condition_variable& cv,
		const Client* client,
		std::string selfDirPath)
		: std::thread(&GameHandler::handle_entierty, this),
		  mutex_(mutex),
		  cv_(cv),
		  m_client(client)
	{
		gameSettings = new GameSettings();
		this->selfDirPath = selfDirPath;
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
		Scene* mainMenu = new MainMenu();
		mainMenu->Init();
		mainMenu->Run();
		dynamic_cast<MainMenu*>(mainMenu)->UpdateGameSettings(gameSettings);
		delete mainMenu;
	}

	void handle_game()
	{
		Scene* game = new Game(gameSettings, m_client);
		game->Init();
		game->Run();
		delete game;
	}

	void handle_multiplayer_if_needed()
	{
		if (gameSettings->m_isMultiplayer)
		{
			cv_.notify_one();
		}
	}

private:
	std::mutex& mutex_;
	std::condition_variable& cv_;

	std::string selfDirPath;

	GameSettings* gameSettings;
	const Client* m_client;
};