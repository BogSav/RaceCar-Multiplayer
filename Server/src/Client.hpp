#pragma once

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <mutex>
#include <thread>

struct TransferStructure
{
	// 4 float-uri = 5 * 4 bytes = 20 bytes
	float x = 0.f;
	float y = 0.f;
	float z = 0.f;

	float angleOrientation = 0.f;

	// Padding 64 bytes - 20 bytes = 44 bytes pentru caching
	//char padding[44];
};

struct InitialDataStructure
{
	std::size_t nrOfPlayers = 0;
	std::size_t clientId = 0;
};

class Client : public std::enable_shared_from_this<Client>
{
public:
	typedef std::shared_ptr<Client> Ptr;
	static inline constexpr std::size_t structureSize = sizeof(TransferStructure);

	static Ptr create(boost::asio::io_context& io_context, std::size_t, class Server*);
	boost::asio::ip::tcp::socket& GetSocket() { return socket_; }
	void Setup();
	void SetOnline();

private:
	using tcp = boost::asio::ip::tcp;
	Client(boost::asio::io_context& io, std::size_t, class Server*);
	void handle_receive();
	void handle_send();

private:
	boost::asio::io_context io_context_;
	boost::asio::strand<boost::asio::io_context::executor_type> strand_;
	tcp::socket socket_;
	boost::array<std::uint8_t, structureSize> buffer;

	const std::size_t client_id;
	std::mutex mtx;
	std::condition_variable cv_;
	bool isOnline;

	TransferStructure client_data;
	InitialDataStructure initial_data;

	class Server* server;
};
