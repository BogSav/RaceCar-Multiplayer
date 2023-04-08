#include "Server.hpp"

#include <boost/bind.hpp>

Server::Server(boost::asio::io_context& io_, std::string ip_adress, long port)
	: io_contex(io_),
	  acceptor_(io_, tcp::endpoint(boost::asio::ip::address::from_string(ip_adress), port))
{
}

void Server::SetOnline()
{
	nr_clienti = 0;

	start_accept();
	io_contex.run();
}

void Server::start_accept()
{
	std::cout << "Se asteapta un nou client" << std::endl;

	std::size_t id = nr_clienti.fetch_add(1);
	Client::Ptr client = Client::create(io_contex, id, this);

	acceptor_.async_accept(
		client->GetSocket(),
		boost::bind(&Server::handle_accept, this, client, boost::asio::placeholders::error));
}

void Server::handle_accept(Client::Ptr client, const boost::system::error_code& error)
{
	if (!error)
	{
		{
			std::lock_guard lock(mtx);
			date_clienti.push_back(TransferStructure());
		}
		client->start();
	}

	start_accept();
}

std::vector<TransferStructure>& Server::SafeAccessDataVector()
{
	std::lock_guard<std::mutex> lock(mtx);
	return date_clienti;
}

void Server::SafeModifyElement(TransferStructure& client_data, std::size_t index)
{
	std::lock_guard<std::mutex> lock(mtx);
	date_clienti[index] = client_data;
}

void Server::SafeAddClient(Client::Ptr client)
{
	std::lock_guard<std::mutex> lock(mtx);
	clienti.push_back(client);
}
