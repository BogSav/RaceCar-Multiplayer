#include "Connection.hpp"

Connection::Connection(std::string ip_adress, long port)
	: std::thread(&Connection::handle_client, this),
	  socket_(io_),
	  resolver_(io_),
	  m_ip_adress_string(ip_adress),
	  m_port(port)
{
}

void Connection::handle_client()
{
	this->wait_unitl_main_thread_ready();

	try
	{
		this->connect_to_server();

		{
			std::jthread receive(&Connection::handle_receive, this);
			std::jthread send(&Connection::handle_send, this);
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
	try
	{
		while (true)
		{
			boost::system::error_code error;
			size_t length = socket_.read_some(boost::asio::buffer(data), error);

			if (error == boost::asio::error::eof)
			{
				std::cout << "Client disconnected: "
						  << socket_.remote_endpoint().address().to_string() << std::endl;
				break;
			}
			else if (error)
			{
				throw boost::system::system_error(error);
			}

			if (length == sizeof(TransferStructure))
			{
				std::lock_guard<std::mutex> lock(mutex_NPC);
				memcpy_s(&NPC_data, sizeof(NPC_data), &data, length);
			}
		}
	}
	catch (std::exception& e)
	{
		throw e;
	}
}

void Connection::handle_send()
{
	// BIG TODO
}

void Connection::connect_to_server()
{
	try
	{
		std::cout << "Se initiaza conexiunea cu serverul..." << std::endl;

		endpoint = resolver_.resolve(m_ip_adress_string, std::to_string(m_port));
		boost::asio::connect(socket_, endpoint);

		{
			std::unique_lock<std::mutex> lock;
			connection_flag = true;
			cv_.notify_one();
		}

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

void Connection::UpdatePositionAndDirection(glm::vec3& pos, float& angleOrientation) const
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
