#include "Client.hpp"

#include "SerializationHelper.hpp"
#include "Server.hpp"

#include <mutex>
#include <thread>
#include <iostream>


Client::Client(boost::asio::io_context& context, std::size_t client_id, Server* server)
	: socket_(context), clientId_(client_id), server_(server)
{
	isOnline_ = false;
}

Client::Ptr Client::create(boost::asio::io_context& context, std::size_t id, Server* server)
{
	return Client::Ptr(new Client(context, id, server));
}

void Client::Setup()
{
	server_->clienti.push_back(shared_from_this());

	std::cerr << "=========================================\n";
	std::cerr << "Client nou conectat cu datele: \n";
	std::cerr << "--> ID: " << clientId_ << "\n";
	std::cerr << "--> Adresa: " << socket_.local_endpoint().address().to_string() << "\n";
	std::cerr << "--> Port: " << socket_.local_endpoint().port() << "\n";
	std::cerr << "=========================================\n";

	jThread = std::jthread(
		[this]
		{
			{
				// Se asteapta pornirea clientului
				std::unique_lock<std::mutex> ulock(mtx_);
				cv_.wait(ulock, [this] { return isOnline_; });
			}
			try
			{
				handle_initial_data();
				handle_receive();

				isOnline_ = false;
				socket_.close();
			}
			catch (std::exception& e)
			{
				std::cerr << e.what();

				isOnline_ = false;
				socket_.close();
			}
		});
}

void Client::SetOnline()
{
	std::cerr << "Clientul " << clientId_ << " este online\n";

	isOnline_ = true;
	cv_.notify_one();
}

void Client::handle_receive()
{
	boost::system::error_code error;
	std::vector<char> serializedData;

	while (true)
	{
		serializedData.clear();
		size_t length = socket_.read_some(boost::asio::buffer(serializedData), error);

		if (error)
		{
			std::cerr << "A aparut o eroare de primire la clientul " << clientId_ << "\n";
			if (error == boost::asio::error::eof)
			{
				std::cerr << "Este posibil ca clientul " << clientId_ << " sa se fii deconectat\n";
			}
			throw boost::system::system_error(error);
		}

		if (length == clientDataSize)
		{
			server_->dateClienti[clientId_] = SerializationHelper::DeserializeClientData(
				std::string(serializedData.begin(), serializedData.end()));
		}
		else
		{
			std::cerr << "S-a trimis ceva ce nu trebuia probabil, cu lungimea asta: " << length
					  << "\n";
			throw std::runtime_error(
				"Clientul " + std::to_string(clientId_) + " a primit un mesaj extra dubios");
		}

		handle_send(error);
	}
}

void Client::handle_send(boost::system::error_code& error)
{
	std::string serializedData = SerializationHelper::SerializeDataArray(server_->dateClienti);
	boost::asio::write(socket_, boost::asio::buffer(boost::asio::buffer(serializedData)), error);

	if (error)
	{
		std::cerr << "A aparut o eroare de trimitere la clientul " << clientId_ << "\n";
		throw boost::system::system_error(error);
	}
}

void Client::handle_initial_data()
{
	boost::system::error_code error;

	std::string st = "BEGIN";
	boost::asio::write(socket_, boost::asio::buffer(st), error);
	if (error)
	{
		std::cerr << "A aparut o eroare la trimiterea mesajului BEGIN catre clientul " << clientId_
				  << "\n";
		throw boost::system::system_error(error);
	}

	ClientInitialData init_data;
	init_data.id = clientId_;
	init_data.nrOfClients = server_->nr_clienti;
	std::string serializedData = SerializationHelper::SerializeClientIntialData(init_data);
	boost::asio::write(socket_, boost::asio::buffer(serializedData), error);
	if (error)
	{
		std::cerr << "A aparut o eroare la trimiterea structurii intiiale catre clientul "
				  << clientId_ << "\n";
		throw boost::system::system_error(error);
	}
}
