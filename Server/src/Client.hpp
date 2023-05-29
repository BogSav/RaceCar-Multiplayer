#pragma once

#include <boost/asio.hpp>
#include "DataArray.hpp"

class Client : public std::enable_shared_from_this<Client>
{
public:
	typedef std::shared_ptr<Client> Ptr;
	using tcp = boost::asio::ip::tcp;

	static Ptr create(boost::asio::io_context&, std::size_t, class Server*);
	boost::asio::ip::tcp::socket& GetSocket() { return socket_; }
	void Setup();
	void SetOnline();
	void SetInitialPosition(float posX, float posY, float posZ);

private:
	Client(boost::asio::io_context&, std::size_t, class Server*);

	void handle_receive();
	void handle_send(boost::system::error_code&);
	void handle_initial_data();

private:
	std::jthread jThread;
	tcp::socket socket_;
	const std::size_t clientId_;

	std::mutex mtx_;
	std::condition_variable cv_;
	bool isOnline_;

	class Server* server_;

	ClientInitialData initialData_;
};
