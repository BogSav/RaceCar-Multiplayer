#include "Client.hpp"

#include "Server.hpp"

#include <boost/asio/steady_timer.hpp>
#include <boost/bind.hpp>

Client::Client(boost::asio::io_context& io, std::size_t client_id, Server* server)
	: socket_(io), strand_(io.get_executor()), client_id(client_id), server(server)
{
	client_data.clientId = static_cast<int>(client_id);
}

Client::Ptr Client::create(boost::asio::io_context& io_context, std::size_t id, Server* server)
{
	return Client::Ptr(new Client(io_context, id, server));
}

bool Client::start()
{
	server->SafeAddClient(shared_from_this());

	handle_receive();
	handle_send();

	std::jthread context([this] { io_context_.run(); });

	return true;
}

void Client::handle_receive()
{
	socket_.async_receive(
		boost::asio::buffer(buffer, structureSize),
		boost::asio::bind_executor(
			strand_,
			[this](boost::system::error_code error, std::size_t length)
			{
				if (error == boost::asio::error::eof)
				{
					std::cout << "Client disconnected: "
							  << socket_.remote_endpoint().address().to_string() << std::endl;
				}
				else if (error)
				{
					std::cout << "A aparut o eroare supicioasa si se inchide conexiunea"
							  << error.what() << std::endl;
					socket_.close();
					return;
				}

				if (length == structureSize)
				{
					memcpy_s(&client_data, structureSize, &buffer, length);
					server->SafeModifyElement(client_data, client_id);
				}

				handle_receive();
			}));
}

void Client::handle_send()
{
	boost::asio::async_write(
		socket_,
		boost::asio::buffer(&server->SafeAccessDataVector(), structureSize * server->nr_clienti),
		boost::asio::bind_executor(
			strand_,
			[this](boost::system::error_code error, std::size_t length)
			{
				if (error)
				{
					std::cout << "A aparut o eroare la trimitere" << error.what() << std::endl;
					socket_.close();
					return;
				}

				handle_send();
			}));
}
