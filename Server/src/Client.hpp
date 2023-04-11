#pragma once

#include "DataBuffer.hpp"

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <mutex>
#include <thread>

class Client : public std::enable_shared_from_this<Client>
{
public:
	typedef std::shared_ptr<Client> Ptr;
	using tcp = boost::asio::ip::tcp;

	static Ptr create(boost::asio::io_context& io_context, std::size_t, class Server*);
	boost::asio::ip::tcp::socket& GetSocket() { return socket_; }
	void Setup();
	void SetOnline();

private:
	struct InitialDataStructure
	{
		std::size_t nrOfPlayers = 0;
		std::size_t clientId = 0;
	};
	Client(boost::asio::io_context& io, std::size_t, class Server*);
	void handle_receive();
	void handle_send();
	void handle_initial_data();

private:
	boost::asio::io_context io_context_;
	boost::asio::strand<boost::asio::io_context::executor_type> strand_;
	tcp::socket socket_;
	boost::array<std::uint8_t, DataBuffer::dataSize> buffer;

	const std::size_t client_id;

	std::mutex mtx;
	std::condition_variable cv_;
	bool isOnline;

	class Server* server;
	DataBuffer& data;
};
