#pragma once

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <mutex>
#include <thread>

struct TransferStructure
{
	// 4 float-uri = 5 * 4 bytes = 20 bytes
	float x;
	float y;
	float z;

	float angleOrientation;

	int clientId;

	// Padding 64 bytes - 20 bytes = 44 bytes pentru caching
	char padding[44];
};

class Client : public std::enable_shared_from_this<Client>
{
public:
	typedef std::shared_ptr<Client> Ptr;
	static inline constexpr std::size_t structureSize = sizeof(TransferStructure);

	static Ptr create(boost::asio::io_context& io_context, std::size_t, class Server*);
	boost::asio::ip::tcp::socket& GetSocket() { return socket_; }
	bool start();

private:
	using tcp = boost::asio::ip::tcp;
	Client(boost::asio::io_context& io, std::size_t, class Server*);

private:
	void handle_receive();
	void handle_send();

private:
	boost::asio::io_context io_context_;
	boost::asio::strand<boost::asio::io_context::executor_type> strand_;
	tcp::socket socket_;
	boost::array<std::uint8_t, structureSize> buffer;

	const std::size_t client_id;
	std::mutex mtx;

	TransferStructure client_data;

	class Server* server;
};
