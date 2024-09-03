#include "../includes/Server.hpp"
#include <signal.h>

int main(int ac, char **av)
{
    try {
        if (ac != 3)
            throw std::runtime_error("./ircserv <port> <password>");
        if (!Utils::port_is_valid(av[1]))
            throw std::runtime_error("invalid port; must be between 1024 and 49151");
        Server *ptr = Server::get_instance();
        signal(SIGINT, Server::signal_handler);
        ptr->manage_server(atoi(av[1]), av[2]);
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}
