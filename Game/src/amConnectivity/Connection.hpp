#pragma once

#include "utils/glm_utils.h"
#include "utils/math_utils.h"

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/asio/strand.hpp>
#include <iostream>
#include <memory_resource>
#include <mutex>
#include <thread>

struct TransferStructure
{
	float x = 0.f;
	float y = 0.f;
	float z = 0.f;

	float angleOrientation = 0.f;

	std::size_t clientId;
};

struct InitialDataStructure
{
	std::size_t nrOfPlayers = 0;
	std::size_t clientId = 0;
};

class Connection : public std::jthread
{
private:
	using tcp = boost::asio::ip::tcp;

public:
	Connection(std::string ip_adress = "192.168.0.186", long port = 25565);

	void UpdateNPCParams(glm::vec3& pos, float& angleOrientation, const std::size_t&) const;
	void UpdateClientParams(const glm::vec3& pos, const float& angleOrientation);

	// Pentru joc
	void StartConnection();
	void PauseGame();

	std::size_t GetClientId() { return clientId; }
	std::vector<TransferStructure>& SafeAccessNPCData();
	const TransferStructure& SafeAccessClientData() const;

	~Connection();

private:
	void handle_client();
	void handle_receive();
	void handle_send();
	void handle_deconnect();

	void connect_to_server();
	void wait_until_npc_is_connected();

	void PauseConnection();
	void StartGame();

private:
	static constexpr std::size_t transferStructureSize = sizeof(TransferStructure);

	std::string m_ip_adress_string;
	long m_port;
	std::size_t clientId;

	std::vector<TransferStructure> NPCs_data;
	TransferStructure client_data;
	InitialDataStructure initial_data;

	boost::asio::io_context io_;
	tcp::socket socket_;
	tcp::resolver resolver_;
	tcp::resolver::results_type endpoint_;
	boost::asio::strand<boost::asio::io_context::executor_type> strand_;

	mutable std::mutex mtx_;
	mutable std::mutex mutex_NPC;
	std::condition_variable cv_;
	bool generic_flag = false;
};