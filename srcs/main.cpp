#include "../includes/Server.hpp"
#include "../includes/Client.hpp"

int main(int argc, char **argv)
{
    Server server;

	 try {
		if (argc != 3)
			throw RuntimeError("Usage: ./server <port_number> <password>");
        server.arg_control(argv);
		server.start();
		signal(SIGINT, server.signal_handler);
    } catch (const RuntimeError& e) {
        std::cout << RED << "Error: " << e.what() << CODE << std::endl;
        return 1;
	} catch (const std::string& str) {
		std::cout << str << std::endl;
		return 0;
	}
    return 0;
}