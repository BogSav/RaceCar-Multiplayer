#include "Connection.hpp"

#include "amGame/GameSettings.hpp"
#include "core/engine.h"

Connection::Connection(SyncHelpper& syncHelpper, std::string ip_adress, long port)
	: std::jthread(&Connection::handle_client, this),
	  socket_(io_),
	  resolver_(io_),
	  m_ip_adress_string(ip_adress),
	  m_port(port),
	  strand_(io_.get_executor()),
	  syncHelpper(syncHelpper)
{
	NPCs_data.push_back(TransferStructure());
	NPCs_data.push_back(TransferStructure());
}

Connection::~Connection()
{
}

void Connection::handle_client()
{
	syncHelpper.PauseConnection();

	try
	{
		connect_to_server();
		wait_until_npc_is_connected();
		syncHelpper.ResumeGame();

		{
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
		boost::asio::buffer(&SafeAccessNPCData(), transferStructureSize * 2),
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

				// if (length == transferStructureSize)
				//{
				//	std::lock_guard<std::mutex> lock(mutex_NPC);
				//	memcpy_s(&NPC_data, transferStructureSize, &NPC_data_buffer, length);
				// }

				handle_send();
			}));
}

void Connection::handle_send()
{
	boost::asio::async_write(
		socket_,
		boost::asio::buffer(&SafeAccessClientData(), transferStructureSize),
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

				handle_receive();
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

void Connection::wait_until_npc_is_connected()
{
	try
	{
		boost::system::error_code ec;

		boost::asio::streambuf buf;
		boost::asio::read_until(socket_, buf, "BEGIN", ec);

		if (ec)
		{
			std::cout << "A aparut o eroare in primirea mesajului de pornire transfer" << std::endl;
		}
		else
		{
			boost::asio::read(
				socket_, boost::asio::buffer(&initial_data, sizeof(initial_data)), ec);

			clientId = initial_data.clientId;
			Engine::GetGameSettings()->m_nrOfPlayers = initial_data.nrOfPlayers;
		}
	}
	catch (std::exception& e)
	{
		throw e;
	}
}

std::vector<TransferStructure>& Connection::SafeAccessNPCData()
{
	std::lock_guard<std::mutex> lock(mtx_);
	return NPCs_data;
}

const TransferStructure& Connection::SafeAccessClientData() const
{
	std::lock_guard<std::mutex> lock(mtx_);
	return client_data;
}

void Connection::UpdateClientParams(const glm::vec3& pos, const float& angleOrientation)
{
	if (mtx_.try_lock())
	{
		client_data.x = pos.x;
		client_data.y = pos.y;
		client_data.z = pos.z;

		client_data.angleOrientation = angleOrientation;

		mtx_.unlock();
	}
	else
	{
		return;
	}
}

void Connection::UpdateNPCParams(
	glm::vec3& pos, float& angleOrientation, const std::size_t& NPC_Id) const
{
	if (mtx_npc.try_lock())
	{
		pos.x = NPCs_data[NPC_Id].x;
		pos.y = NPCs_data[NPC_Id].y;
		pos.z = NPCs_data[NPC_Id].z;

		angleOrientation = NPCs_data[NPC_Id].angleOrientation;

		mtx_npc.unlock();
	}
	else
	{
		return;
	}
}