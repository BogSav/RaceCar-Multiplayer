#include <ctime>
#include <iostream>

#include "amGame/GameHandler.hpp"

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <cstdlib>

#ifdef _WIN32
PREFER_DISCRETE_GPU_NVIDIA;
PREFER_DISCRETE_GPU_AMD;
#endif

std::string GetParentDir(const std::string& filePath)
{
	size_t pos = filePath.find_last_of("\\/");
	return (std::string::npos == pos) ? "." : filePath.substr(0, pos);
}

int main(int argc, char** argv)
{
	//_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	//_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
	//_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
	//_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
	//_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
	//_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);

	srand((unsigned int)time(NULL));

	SyncHelpper helper;

	Connection* connection = new Connection{helper, "192.168.0.186", 25500};

	Engine::SetConnection(connection);
	Engine::SetGameSettings(new GameSettings());

	GameHandler* gameHandler = new GameHandler(helper, GetParentDir(std::string(argv[0])));

	//connection->join();

	delete gameHandler;
	delete connection;

	//_CrtDumpMemoryLeaks();
	return 0;
}
