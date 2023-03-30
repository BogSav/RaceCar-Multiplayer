#pragma once

#include "amConnectivity/Connection.hpp"
#include "amGame/Game.hpp"
#include "amUI/MainMenu.hpp"

#include <iostream>
#include <mutex>
#include <thread>

class GameHandler : public std::thread
{
public:
	GameHandler(Connection* connection, std::string selfDirPath)
		: std::thread(&GameHandler::handle_entierty, this),
		  m_connection(connection),
		  mutex_(connection->GetClientMutex()),
		  cv_(connection->GetConditionVariable())
	{
		Engine::SetGameSettings(new GameSettings());
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
		dynamic_cast<MainMenu*>(mainMenu)->UpdateGameSettings();
		delete mainMenu;
	}

	void handle_game()
	{
		Scene* game = new Game();
		game->Init();
		game->Run();
		delete game;
	}

	void handle_multiplayer_if_needed()
	{
		if (Engine::GetGameSettings()->m_isMultiplayer)
		{
			{
				std::lock_guard<std::mutex> lock(mutex_);
				m_connection->SetMultiplayerFlag(true);
				cv_.notify_one();
			}

			{
				std::unique_lock<std::mutex> lock(mutex_);
				while (!m_connection->GetConnectionFlag())
				{
					cv_.wait(lock);
				}
			}
		}
	}

private:
	std::mutex& mutex_;
	std::condition_variable& cv_;

	std::string selfDirPath;

	Connection* m_connection;
};