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
		
    } catch (const RuntimeError& e) {
        std::cerr << "Error: " << e.what() << std::endl;

        return 1;
    }
    return 0;
}