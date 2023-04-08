#include "Client.hpp"
#include "Server.hpp"

std::string ip_adress = "192.168.56.1";
long port = 25565;

int main()
{
	boost::asio::io_context io;
	try
	{
		Server server(io, ip_adress, port);
		server.SetOnline();
	}
	catch (std::exception& e)
	{
		std::cout << "Ceva merge prost" << std::endl;
		std::cout << e.what() << std::endl;
	}

	return 0;
}