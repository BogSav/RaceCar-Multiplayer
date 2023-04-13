﻿#include "Client.hpp"

#include "Server.hpp"

#include <boost/asio/steady_timer.hpp>
#include <boost/bind.hpp>

Client::Client(boost::asio::io_context& c, std::size_t client_id, Server* server)
	: socket_(c), client_id(client_id), server(server), data(server->data)
{
	isOnline = false;
}

Client::Ptr Client::create(boost::asio::io_context& c, std::size_t id, Server* server)
{
	return Client::Ptr(new Client(c, id, server));
}

void Client::Setup()
{
	server->clienti.push_back(shared_from_this());

	std::cout << "Client nou conectat cu datele: \n";
	std::cout << "--> ID: " << client_id << "\n";
	std::cout << "--> Adresa: " << socket_.local_endpoint().address().to_string() << "\n";
	std::cout << "--> Port: " << socket_.local_endpoint().port() << "\n\n";

	j = std::jthread(
		[this]
		{
			{
				std::unique_lock<std::mutex> ulock(mtx);
				cv_.wait(ulock, [this] { return isOnline; });
			}
			try
			{
				handle_initial_data();
				handle_receive();

				socket_.close();
			}
			catch (std::exception& e)
			{
				std::cerr << e.what();
			}
		});
}

void Client::SetOnline()
{
	std::cout << "Clientul " << client_id << " este online\n";

	isOnline = true;
	cv_.notify_one();
}

void Client::handle_receive()
{
	boost::system::error_code error;
	while (true)
	{
		size_t length = socket_.read_some(boost::asio::buffer(buffer), error);

		if (error == boost::asio::error::eof)
		{
			std::cout << "Client disconnected: " << socket_.remote_endpoint().address().to_string()
					  << std::endl;
			break;
		}
		else if (error)
		{
			throw boost::system::system_error(error);
		}

		if (length == DataBuffer::dataSize)
		{
			data.UpdateElement(buffer, client_id);
			std::cout << "Primit ok\n";
		}
		else
		{
			std::cout << "Ce drq s-a trimis??" << length << "\n";
		}

		handle_send(error);
	}
}

void Client::handle_send(boost::system::error_code& error)
{
	boost::asio::write(
		socket_, boost::asio::buffer(&data.GetDataVector(), DataBuffer::dataSize * 2), error);

	if (error)
	{
		std::cerr << "Eroare las trimitere client " << client_id << " " << error.what();
	}
}

void Client::handle_initial_data()
{
	try
	{
		std::string st = "BEGIN";
		boost::asio::write(socket_, boost::asio::buffer(st));

		InitialDataStructure init_data;
		init_data.clientId = client_id;
		init_data.nrOfPlayers = server->nr_clienti;
		boost::asio::write(socket_, boost::asio::buffer(&init_data, sizeof(init_data)));
	}
	catch (std::exception& e)
	{
		throw e;
	}
}
