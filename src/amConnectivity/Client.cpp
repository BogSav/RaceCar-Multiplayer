#include "Client.hpp"

Client::Client(std::mutex& mutex, std::condition_variable& cv, std::string ip_adress, long port)
	: std::thread(&Client::handle_client, this),
	  socket_(io_),
	  resolver_(io_),
	  m_ip_adress_string(ip_adress),
	  m_port(port),
	  mutex_(mutex),
	  cv_(cv)
{
}

void Client::UpdatePositionAndDirection(glm::vec3& pos, float& angleOrientation) const
{
	if (mutex_.try_lock())
	{
		pos.x = params.x;
		pos.y = params.y;
		pos.z = params.z;

		angleOrientation = params.angleOrientation;

		mutex_.unlock();
	}
	else
	{
		return;
	}
}

void Client::handle_client()
{
	this->wait_unitl_main_thread_ready();

	try
	{
		this->connect_to_server();

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

			if (length == sizeof(params))
			{
				std::lock_guard<std::mutex> lock(mutex_);
				memcpy_s(&params, sizeof(params), &data, length);
			}
		}
	}
	catch (std::exception& e)
	{
		std::cout << "Se opreste clientul din cazua erorii: " << e.what() << std::endl;
	}
}

void Client::connect_to_server()
{
	try
	{
		std::cout << "Se initiaza conexiunea cu serverul..." << std::endl;

		endpoint = resolver_.resolve(m_ip_adress_string, std::to_string(m_port));
		boost::asio::connect(socket_, endpoint);

		std::cout << "Conexiune realizata cu succes" << std::endl;
	}
	catch (std::exception& e)
	{
		std::cout << "Nu a putut fii realizata conexiunea" << std::endl;
		throw e;
	}
}

void Client::wait_unitl_main_thread_ready()
{
	std::cout << "S-a creat clientul, acum asteptam deblocarea thread-ului..." << std::endl;

	std::unique_lock<std::mutex> lock(mutex_);
	cv_.wait(lock);

	std::cout << "Thread-ul a fost deblocat, se continua executia..." << std::endl;
}
