#pragma once

/*
 *  Graphic Engine
 */

#include "core/window/window_object.h"

class Engine
{
public:
	static WindowObject* Init(const WindowProperties& props);
	static WindowObject* GetWindow();

	static void SetGameSettings(GameSettings*);
	static GameSettings* GetGameSettings();

	static void SetConnection(Connection*);
	static Connection* GetConnection();

	// Get elapsed time in seconds since the application started
	static double GetElapsedTime();

	static void Exit();

private:
	static WindowObject* window;
	static GameSettings* settings;
	static Connection* connection;
};
