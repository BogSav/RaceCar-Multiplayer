#pragma once

#include "utils/glm_utils.h"
#include "utils/math_utils.h"

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <mutex>
#include <thread>

using boost::asio::ip::tcp;

class Connection : public std::thread
{
private:
	struct TransferStructure
	{
		float x = 10;
		float y = 10;
		float z = 10;

		float angleOrientation = 0;
	};

public:
	Connection(std::string ip_adress = "192.168.0.186", long port = 25565);

	void UpdatePositionAndDirection(glm::vec3& pos, float& angleOrientation) const;
	void UpdateClientParams(const glm::vec3& pos, const float& angleOrientation);

	std::mutex& GetClientMutex() { return mutex_client; }
	std::condition_variable& GetConditionVariable() { return cv_; }

	void SetMultiplayerFlag(bool toSet) { multiplayer_flag = toSet; }
	bool GetConnectionFlag() { return connection_flag; }

private:
	void handle_client();
	void handle_receive();
	void handle_send();
	void handle_deconnect(); // TODO

	void connect_to_server();
	void wait_unitl_main_thread_ready();

private:
	std::string m_ip_adress_string;
	long m_port;

	TransferStructure NPC_data;
	TransferStructure client_data;

	boost::asio::io_context io_;
	tcp::socket socket_;
	tcp::resolver resolver_;
	tcp::resolver::results_type endpoint;

	boost::array<uint8_t, 1024> data;

	std::mutex mutex_client;
	mutable std::mutex mutex_NPC;

	std::condition_variable cv_;
	bool multiplayer_flag = false;
	bool connection_flag = false;
};