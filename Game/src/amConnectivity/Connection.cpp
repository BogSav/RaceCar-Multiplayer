#include "Connection.hpp"

#include "amGame/GameSettings.hpp"
#include "core/engine.h"

#include <algorithm>
#include <ranges>

Connection::Connection(SyncHelpper& syncHelpper, std::string ip_adress, long port)
	: std::jthread(&Connection::handle_client, this),
	  socket_(io_),
	  resolver_(io_),
	  m_ip_adress_string(ip_adress),
	  m_port(port),
	  syncHelpper(syncHelpper),
	  isFullyOnline(false),
	  serializedData_(maxNumberOfClients * clientDataSize, 0)
{
	std::lock_guard<std::mutex> lock(mtx_);
	clientData_.id = clientId_;
}

Connection::~Connection()
{
}

void Connection::handle_client()
{
	syncHelpper.PauseConnection();

	// if (m_forceStop)
	//{
	//	return;
	// }

	try
	{
		connect_to_server();
		wait_until_npc_is_connected();

		handle_send();

		socket_.close();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what();
		socket_.close();
	}
}

void Connection::handle_send()
{
	boost::system::error_code error;
	std::string serializedData;

	while (true)
	{
		{
			std::lock_guard<std::mutex> lock(mtx_);
			serializedData = SerializationHelper::SerializeClientData(clientData_);
		}

		std::size_t dim = serializedData.size();
		boost::asio::write(socket_, boost::asio::buffer(&dim, sizeof(dim)), error);
		boost::asio::write(socket_, boost::asio::buffer(serializedData), error);

		if (error)
		{
			std::cerr << "A aparut o eroare la trimiterea datelor\n";
			throw boost::system::system_error(error);
		}

		handle_receive(error);
	}
}

void Connection::handle_receive(boost::system::error_code& error)
{
	std::size_t dim = 0;
	boost::asio::read(socket_, boost::asio::buffer(&dim, sizeof(dim)), error);

	serializedData_ = std::vector<char>(dim, 0);
	boost::asio::read(socket_, boost::asio::buffer(serializedData_), error);

	if (error)
	{
		std::cerr << "A aparut o eroare de primire la clientul " << clientId_ << "\n";
		if (error == boost::asio::error::eof)
		{
			std::cerr << "Este posibil ca clientul " << clientId_ << " sa se fii deconectat\n";
		}
		throw boost::system::system_error(error);
	}

	if (dim != 0)
	{
		data_ = SerializationHelper::DeserializeDataArray<ClientData, maxNumberOfClients>(
			std::string(serializedData_.begin(), serializedData_.end()));

		if (!isFullyOnline)
		{
			isFullyOnline = true;
			syncHelpper.ResumeGame();
		}
	}
	else
	{
		std::cerr << "S-a trimis ceva ce nu trebuia probabil, cu lungimea asta: " << dim << "\n";
		throw std::runtime_error("Clientul a primit un mesaj extra dubios");
	}
}

void Connection::handle_deconnect()
{
	// TODO...
}

void Connection::connect_to_server()
{
	std::cerr << "Se initiaza conexiunea cu serverul..." << std::endl;

	boost::system::error_code error;
	endpoint_ = resolver_.resolve(m_ip_adress_string, std::to_string(m_port));
	boost::asio::connect(socket_, endpoint_, error);

	if (error)
	{
		std::cerr << "A aparut o eroare la conectare ala server\n";
		throw boost::system::system_error(error);
	}
	else
	{
		std::cerr << "Conexiune realizata cu succes, se continua..." << std::endl;
	}
}

void Connection::wait_until_npc_is_connected()
{
	boost::system::error_code error;

	std::size_t message_size = 0;
	boost::asio::read(socket_, boost::asio::buffer(&message_size, sizeof(message_size)));

	std::vector<char> message_buffer(message_size);
	boost::asio::read(socket_, boost::asio::buffer(message_buffer));

	std::string s(message_buffer.begin(), message_buffer.end());
	if (error || s != "BEGIN")
	{
		std::cerr << "A aparut o eroare in primirea mesajului BEGIN\n";
		throw boost::system::system_error(error);
	}

	std::size_t data_size = 0;
	boost::asio::read(socket_, boost::asio::buffer(&data_size, sizeof(data_size)), error);

	serializedData_ = std::vector<char>(data_size, 0);
	boost::asio::read(socket_, boost::asio::buffer(serializedData_), error);
	if (error)
	{
		std::cerr << "A aparut o eroare in primirea structurii cu datele inititale\n";
		throw boost::system::system_error(error);
	}
	else
	{
		ClientInitialData aux = SerializationHelper::DeserializeClientInitialData(
			std::string(serializedData_.begin(), serializedData_.end()));

		assert(aux.id < 3 && aux.id >= 0);
		assert(aux.nrOfClients <= maxNumberOfClients);

		clientId_ = aux.id;
		Engine::GetGameSettings()->m_nrOfPlayers = aux.nrOfClients;
		Engine::GetGameSettings()->GetCarParameters().startPosition =
			glm::vec3{aux.posX, aux.posY, aux.posZ};

		std::cerr << "S-au primit datele initiale\n";
	}
}

void Connection::UpdateClientParams(
	const glm::vec3& pos,
	const float& angleOrientation,
	const std::size_t& positionOnTrack,
	const uint8_t& lapNr)
{
	if (mtx_.try_lock())
	{
		clientData_.posX = pos.x;
		clientData_.posY = pos.y;
		clientData_.posZ = pos.z;

		clientData_.OXangle = angleOrientation;

		clientData_.lapNr = lapNr;
		clientData_.positionOnTrack = positionOnTrack;

		mtx_.unlock();
	}
	else
	{
		return;
	}
}

void Connection::UpdateNPCParams(
	glm::vec3& pos,
	float& angleOrientation,
	std::size_t& positionOnTrack,
	uint8_t& lapNr,
	const std::size_t& NPC_Id) const
{
	const ClientData& data = data_[NPC_Id];
	pos.x = data.posX;
	pos.y = data.posY;
	pos.z = data.posZ;
	angleOrientation = data.OXangle;
	lapNr = data.lapNr;
	positionOnTrack = data.positionOnTrack;
}
