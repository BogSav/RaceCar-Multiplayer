#include "Server.hpp"

#include <boost/bind/bind.hpp>
#include <iostream>
#include <vector>
#include <tuple>

Server::Server(boost::asio::io_context& io_, std::string ip_adress, long port)
	: io_contex_(io_),
	  acceptor_(
		  io_,
		  tcp::endpoint(
			  boost::asio::ip::address::from_string(ip_adress),
			  static_cast<boost::asio::ip::port_type>(port)))
{
	std::cout << "Server initiat cu succes la adresa: " << ip_adress << " : " << port << "\n";
}

void Server::SetOnline()
{
	nr_clienti = 0;

	start_accept();
	io_contex_.run();
}

void Server::start_accept()
{
	std::cout << "Se asteapta un nou client..\n";

	std::size_t id = nr_clienti++;
	Client::Ptr client = Client::create(io_contex_, id, this);

	acceptor_.async_accept(
		client->GetSocket(),
		boost::bind(&Server::handle_accept, this, client, boost::placeholders::_1));
}

void Server::handle_accept(Client::Ptr client, const boost::system::error_code& error)
{
	if (!error)
	{
		client->Setup();
	}
	else
	{
		std::cout << "A aparut o eroare suspicioasa: " << error.what();
		return;
	}

	if (nr_clienti == maxNumberOfClients)
	{
		handle_start_game();
	}
	else
	{
		start_accept();
	}
}

void Server::handle_start_game()
{
	std::cout << "S-au conectat suficienti playeri..." << std::endl;

	std::vector<std::tuple<float, float, float>> v;
	v.push_back({-10.0f, -0.1f, -40.0f});
	v.push_back({-37.0f, -0.1f, -47.0f});

	size_t i = 0;
	for (auto& client : clienti)
	{
		client->SetInitialPosition(std::get<0>(v[i]), std::get<1>(v[i]), std::get<2>(v[i]));
		client->SetOnline();

		i++;
	}

	std::cout << "Se opreste acceptarea de noi clienti\n";
}
