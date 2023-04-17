#pragma once

#include "utils/glm_utils.h"
#include "utils/math_utils.h"
#include "SyncHelpper.hpp"
#include "../../Server/src/SerializationHelper.hpp"

class Connection : public std::jthread
{
private:
	using tcp = boost::asio::ip::tcp;

public:
	Connection(SyncHelpper& syncHelpper, std::string ip_adress = "192.168.0.186", long port = 25565);

	void UpdateNPCParams(glm::vec3& pos, float& angleOrientation, const std::size_t&) const;
	void UpdateClientParams(const glm::vec3& pos, const float& angleOrientation);

	std::size_t GetClientId() { return clientId_.load(); }
	bool GetOnlineStatus() { return isFullyOnline; }

	~Connection();

private:
	void handle_client();
	void handle_receive(boost::system::error_code& error);
	void handle_send();
	void handle_deconnect();

	void connect_to_server();
	void wait_until_npc_is_connected();

private:
	const std::string m_ip_adress_string;
	const long m_port;

	std::atomic<std::size_t> clientId_;
	boost::asio::io_context io_;
	tcp::socket socket_;
	tcp::resolver resolver_;
	tcp::resolver::results_type endpoint_;

	DataArray<ClientData, maxNumberOfClients> data_;
	ClientData clientData_;
	std::vector<char> serializedData_;

	bool isFullyOnline;

	mutable std::mutex mtx_;

	SyncHelpper& syncHelpper;
};