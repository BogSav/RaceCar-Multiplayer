#include <ctime>
#include <iostream>

#if defined(GAME)
#include "amGame/GameHandler.hpp"
#endif

#if defined(CONNECTIVITY)
#include "amConnectivity/Client.hpp"
#endif

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

	std::mutex mutex_;
	std::condition_variable cv;

	Client client(mutex_, cv);
	GameHandler game(mutex_, cv, &client, GetParentDir(std::string(argv[0])));

	game.join();
	client.join();

	//_CrtDumpMemoryLeaks();

	return 0;
}
