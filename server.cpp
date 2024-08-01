#include "server.hpp"

void Server::arg_control(char **argv)
{
	int port_number = 0;

	if (argv[1] != NULL) {
		port_number = std::atoi(argv[1]);
	} else {
		throw RuntimeError("Usage: ./server <port_number>");
	}
	std::cout << "Port number: " << port_number << std::endl;
	if (port_number <= 0)
	{
		throw RuntimeError("Invalid port number.");
	}
	exit(2);
}



// int	Server::socket_init(int port_number)
// {
// 	this->socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
// 	if (this->socket_fd < 0)
// 	{
// 		std::cerr << "Socket creation failed." << std::endl;
// 		return 1;
// 	}

// 	std::memset(&socket_feature, 0, sizeof(socket_feature));
// 	socket_feature.sin_family = AF_INET;
// 	socket_feature.sin_port = htons(port_number);
// 	socket_feature.sin_addr.s_addr = inet_addr("127.0.0.1");

// 	if (bind(this->socket_fd, (struct sockaddr *)&socket_feature, sizeof(socket_feature)) < 0)
// 	{
// 		std::cerr << "Bind failed." << std::endl;
// 		close(this->socket_fd);
// 		return 1;
// 	}
// 	if (listen(this->socket_fd, 5) < 0)
// 	{
// 		std::cerr << "Listen failed." << std::endl;
// 		close(this->socket_fd);
// 		return 1;
// 	}

// 	std::cout << "Waiting for connections..." << std::endl;


// }
#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>
#include <exception>
#include <string>

class RuntimeError : public std::exception {
	private:
		   std::string message;

	public:
		RuntimeError(const std::string& msg) : message(msg) {}

		virtual ~RuntimeError() throw() {}

		virtual const char* what() const throw() {
			  return message.c_str();
		}
};

class Server
{
	private:
		int socket_fd;
		// Diğer üyeler...

	public:
		void arg_control(char **argv);
};

#endif // SERVER_HPP