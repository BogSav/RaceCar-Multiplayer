#pragma once

#include "utils/glm_utils.h"
#include "utils/math_utils.h"

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/asio/strand.hpp>

#include <iostream>
#include <mutex>
#include <thread>
#include <memory_resource>

class Connection : public std::jthread
{
private:
	using tcp = boost::asio::ip::tcp;
	struct TransferStructure
	{
		float x = 10;
		float y = 10;
		float z = 10;

		float angleOrientation = 0;
	};

public:
	Connection(std::string ip_adress = "192.168.0.186", long port = 25565);

	void UpdateNPCParams(glm::vec3& pos, float& angleOrientation) const;
	void UpdateClientParams(const glm::vec3& pos, const float& angleOrientation);

	std::mutex& GetClientMutex() { return mutex_client; }
	std::condition_variable& GetConditionVariable() { return cv_; }

	void SetMultiplayerFlag(bool toSet) { multiplayer_flag = toSet; }
	bool GetConnectionFlag() { return connection_flag; }

	~Connection();

private:
	void handle_client();
	void handle_receive();
	void handle_send();
	void handle_deconnect();

	void connect_to_server();
	void wait_unitl_main_thread_ready();
	void wait_until_npc_is_connected();
	void unlock_main_thread();

	const TransferStructure& getClientData() const;

private:
	static constexpr std::size_t transferStructureSize = sizeof(TransferStructure);

	std::string m_ip_adress_string;
	long m_port;



	TransferStructure NPC_data;
	TransferStructure client_data;

	boost::asio::io_context io_;
	tcp::socket socket_;
	tcp::resolver resolver_;
	tcp::resolver::results_type endpoint_;
	boost::asio::strand<boost::asio::io_context::executor_type> strand_;

	boost::array<uint8_t, transferStructureSize> NPC_data_buffer;

	mutable std::mutex mutex_client;
	mutable std::mutex mutex_NPC;

	std::condition_variable cv_;
	bool multiplayer_flag = false;
	bool connection_flag = false;
};