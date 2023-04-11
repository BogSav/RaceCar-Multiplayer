#pragma once
#include "Client.hpp"

#include <boost/asio.hpp>

class Server
{
private:
	using tcp = boost::asio::ip::tcp;

public:
	Server(boost::asio::io_context& io_, std::string ip_adress, long port);
	void SetOnline();

private:
	tcp::acceptor acceptor_;
	boost::asio::io_context& io_contex_;

	std::mutex mtx_date_clienti_;
	std::mutex mtx_clienti_;

	void start_accept();
	void handle_accept(Client::Ptr client, const boost::system::error_code& error);

	friend Client;

public:
	std::vector<Client::Ptr> clienti;
	std::atomic<std::size_t> nr_clienti;
	DataBuffer data;
};
