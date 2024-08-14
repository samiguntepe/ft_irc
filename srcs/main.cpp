#include "../includes/Server.hpp"
#include "../includes/Client.hpp"
#include "../includes/Bot.hpp"

int main(int argc, char **argv)
{
	
	try {
		if (argc != 3)
			throw RuntimeError("Usage: ./server <port_number> <password>");
		Server	server(AF_INET, SOCK_STREAM, "First IRC");
		server.arg_control(argv);
		server.start();
	} catch (const RuntimeError& e) {
        std::cout << RED << "Error: " << e.what() << CODE << std::endl;
        return 1;
	} catch (const std::string& str) {
		std::cout << str << std::endl;
		return 0;
	}
	return 0;
}
