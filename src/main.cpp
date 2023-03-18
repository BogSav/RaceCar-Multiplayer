#include <ctime>
#include <iostream>

#include "core/engine.h"
#include "components/simple_scene.h"

//#if defined(GAME)
//#   include "Game/Game.hpp"
//#endif

#if defined(UI)
#   include "amUI/MainMenu.hpp"
#endif

#if defined(GAME)
#	include "amGame/Game.hpp"
#endif

#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#ifdef _WIN32
    PREFER_DISCRETE_GPU_NVIDIA;
    PREFER_DISCRETE_GPU_AMD;
#endif


std::string GetParentDir(const std::string &filePath)
{
    size_t pos = filePath.find_last_of("\\/");
    return (std::string::npos == pos) ? "." : filePath.substr(0, pos);
}

int main(int argc, char **argv)
{
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);

	srand((unsigned int)time(NULL));

	// Create a window property structure
	WindowProperties wp;
	wp.resolution = glm::ivec2(1280, 720);
	wp.vSync = true;
	wp.selfDir = GetParentDir(std::string(argv[0]));

	// Init the Engine and create a new window with the defined properties
	(void)Engine::Init(wp);

	GameSettings* gameSettings = new GameSettings();

	{
		Scene* mainMenu = new MainMenu();
		mainMenu->Init();
		mainMenu->Run();
		dynamic_cast<MainMenu*>(mainMenu)->UpdateGameSettings(gameSettings);
		delete mainMenu;
	}

	{
		Scene* game = new Game(gameSettings);
		game->Init();
		game->Run();
	}

	Engine::Exit();

	//_CrtDumpMemoryLeaks();

    return 0;
}
