#include "Server.hpp"

#include <boost/bind/bind.hpp>

Server::Server(boost::asio::io_context& io_, std::string ip_adress, long port)
	: io_contex_(io_),
	  acceptor_(
		  io_,
		  tcp::endpoint(
			  boost::asio::ip::address::from_string(ip_adress),
			  static_cast<boost::asio::ip::port_type>(port))),
	  data(2)
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

#ifdef USE_VECTOR
	data.CreateNewElement(id);
#endif

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

	if (nr_clienti == 1)
	{
		std::cout << "S-au conectat suficienti playeri..." << std::endl;
		for (auto& client : clienti)
		{
			client->SetOnline();
		}
		std::cout << "Se opreste acceptarea de noi clienti\n";
	}
	else
	{
		start_accept();
	}
}
