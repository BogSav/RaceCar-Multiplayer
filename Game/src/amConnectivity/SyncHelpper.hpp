#pragma once

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/asio/strand.hpp>
#include <iostream>
#include <memory_resource>
#include <mutex>
#include <thread>

class SyncHelpper
{
public:
	// Functii start-stop game
	void PauseGame() const
	{
		std::unique_lock<std::mutex> lock(gameMtx);
		gameCv_.wait(lock, [this] { return gameOn; });
	}
	void ResumeGame()
	{
		gameOn = true;
		gameCv_.notify_one();
	}

	// Functii start-stop conexiune
	void PauseConnection()
	{
		std::cerr << "S-a creat clientul, acum asteptam deblocarea thread-ului..." << std::endl;
		{
			std::unique_lock<std::mutex> lock(connectionMtx);
			connectionCv_.wait(lock, [this] { return connectionOn; });
		}
		std::cerr << "Thread-ul a fost deblocat, se continua cu conexiunea..." << std::endl;
	}
	void ResumeConnection()
	{
		connectionOn = true;
		connectionCv_.notify_one();
	}

	// Altele
	void ResetFlags(bool val = false)
	{
		connectionOn = val;
		gameOn = val;
	}

private:
	mutable std::mutex connectionMtx;
	mutable std::condition_variable connectionCv_;

	mutable std::mutex gameMtx;
	mutable std::condition_variable gameCv_;

	bool connectionOn = false;
	bool gameOn = false;
};