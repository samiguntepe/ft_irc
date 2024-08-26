#include "../includes/Server.hpp"
#include "../includes/Client.hpp"
#include "../includes/Bot.hpp"

int main(int ac, char **av)
{
	if (ac != 3) {
		write(2, "Usage: ./server <port> <password>\n", 34);
		return 1;
	}
	int port = std::atoi(av[1]);
	if (port < 1024 || port > 65535)
	{
		write(2, "Invalid port\n", 13);
		return 1;
	}
	std::string password = av[2];
	try {
		Server	srv(AF_INET, SOCK_STREAM, port, "First IRC");
		srv.set_server_password(password);
		srv.server_run();
	}
	catch (std::exception& e) {
		write(2, e.what(), strlen(e.what()));
		return 1;
	}
	return 0;
}
