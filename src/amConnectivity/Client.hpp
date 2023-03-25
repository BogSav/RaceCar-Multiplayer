#pragma once

#include "amGame/GameComponents/Car.hpp"
#include "utils/glm_utils.h"
#include "utils/math_utils.h"

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <mutex>
#include <thread>

using boost::asio::ip::tcp;

class Client : public std::thread
{
private:
	struct position_and_direction
	{
		float x = 10;
		float y = 10;
		float z = 10;

		float angleOrientation;
	};

public:
	Client(
		std::mutex& mutex,
		std::condition_variable& cv,
		std::string ip_adress = "192.168.0.186",
		long port = 25565);

	void UpdatePositionAndDirection(glm::vec3& pos, float& angleOrientation) const;

private:
	void handle_client();
	void connect_to_server();
	void wait_unitl_main_thread_ready();

private:
	std::string m_ip_adress_string;
	long m_port;

	position_and_direction params;

	boost::asio::io_context io_;
	tcp::socket socket_;
	tcp::resolver resolver_;
	tcp::resolver::results_type endpoint;

	boost::array<uint8_t, 1024> data;

	std::mutex& mutex_;
	std::condition_variable& cv_;
};