#include "Connection.hpp"

Connection::Connection(std::string ip_adress, long port)
	: std::thread(&Connection::handle_client, this),
	  socket_(io_),
	  resolver_(io_),
	  m_ip_adress_string(ip_adress),
	  m_port(port),
	  strand_(io_.get_executor())
{
}

void Connection::handle_client()
{
	wait_unitl_main_thread_ready();

	try
	{
		connect_to_server();
		wait_until_npc_is_connected();
		unlock_main_thread();

		{
			handle_receive();
			handle_send();
			std::jthread context([this]() { io_.run(); });
		}

		std::cout << "Se inchide conexiunea cu server-ul..." << std::endl;
	}
	catch (std::exception& e)
	{
		std::cout << "Se opreste clientul din cazua erorii: " << e.what() << std::endl;
	}
}

void Connection::handle_receive()
{
	socket_.async_receive(
		boost::asio::buffer(NPC_data_buffer, transferStructureSize),
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

				if (length == transferStructureSize)
				{
					std::lock_guard<std::mutex> lock(mutex_NPC);
					memcpy_s(&NPC_data, transferStructureSize, &NPC_data_buffer, length);
				}

				handle_receive();
			}));
}

void Connection::handle_send()
{
	boost::asio::async_write(
		socket_,
		boost::asio::buffer(&getClientData(), transferStructureSize),
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

void Connection::handle_deconnect()
{
	// TODO...
}

void Connection::connect_to_server()
{
	try
	{
		std::cout << "Se initiaza conexiunea cu serverul..." << std::endl;

		endpoint_ = resolver_.resolve(m_ip_adress_string, std::to_string(m_port));
		boost::asio::connect(socket_, endpoint_);

		std::cout << "Conexiune realizata cu succes, se continua..." << std::endl;
	}
	catch (std::exception& e)
	{
		std::cout << "Nu a putut fii realizata conexiunea" << std::endl;
		throw e;
	}
}

void Connection::wait_unitl_main_thread_ready()
{
	std::cout << "S-a creat clientul, acum asteptam deblocarea thread-ului..." << std::endl;

	std::unique_lock<std::mutex> lock(mutex_client);
	while (!multiplayer_flag)
	{
		cv_.wait(lock);
	}

	std::cout << "Thread-ul a fost deblocat, se continua cu conexiunea..." << std::endl;
}

void Connection::wait_until_npc_is_connected()
{
	try
	{
		boost::asio::streambuf buf;
		boost::system::error_code ec;
		boost::asio::read_until(socket_, buf, "BEGIN_TRANSFER", ec);

		if (ec)
		{
			std::cout << "A aparut o eroare in primirea mesajului de pornire transfer" << std::endl;
		}
	}
	catch (std::exception& e)
	{
		throw e;
	}
}

void Connection::unlock_main_thread()
{
	std::unique_lock<std::mutex> lock;
	connection_flag = true;
	cv_.notify_one();
}

const Connection::TransferStructure& Connection::getClientData() const
{
	std::lock_guard<std::mutex> lock(mutex_client);
	return client_data;
}

void Connection::UpdateClientParams(const glm::vec3& pos, const float& angleOrientation)
{
	if (mutex_client.try_lock())
	{
		client_data.x = pos.x;
		client_data.y = pos.y;
		client_data.z = pos.z;

		client_data.angleOrientation = angleOrientation;

		mutex_client.unlock();
	}
	else
	{
		return;
	}
}

void Connection::UpdateNPCParams(glm::vec3& pos, float& angleOrientation) const
{
	if (mutex_NPC.try_lock())
	{
		pos.x = NPC_data.x;
		pos.y = NPC_data.y;
		pos.z = NPC_data.z;

		angleOrientation = NPC_data.angleOrientation;

		mutex_NPC.unlock();
	}
	else
	{
		return;
	}
}
