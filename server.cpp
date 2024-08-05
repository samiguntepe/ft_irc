#include "server.hpp"

void Server::run()
{
	create_socket();
	bind_listen_socket();
	accept_select_socket();
}

void Server::arg_control(char **argv)
{
	port_number = 0;

	if (argv[1] != NULL) {
		port_number = std::atoi(argv[1]);
	} else {
		throw RuntimeError("Usage: ./server <port_number>");
	}
	if (port_number <= 0)
	{
		throw RuntimeError("Invalid port number.");
	}
}

void	Server::create_socket()
{
	this->server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (this->server_fd < 0)
		throw RuntimeError("Socket creation failed.");

	std::memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(port_number);
	server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
	this->addr_len = sizeof(server_address);
}

void Server::bind_listen_socket()
{
	if (bind(this->server_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
	{
		close(this->server_fd);
		throw RuntimeError("Bind failed.");
	}
	if (listen(this->server_fd, 42) < 0)
	{
		close(this->server_fd);
		throw RuntimeError("Listen failed.");
	}
	std::cout << "Waiting for connections..." << std::endl;
}

void Server::accept_select_socket()
{
	int new_socket;
	fd_set read_fds;
	FD_ZERO(&read_fds);
	FD_SET(server_fd, &read_fds);
	int max_fd = server_fd;

	while (1) {
    fd_set temp_fds = read_fds;

    int activity = select(max_fd + 1, &temp_fds, NULL, NULL, NULL);

    if (activity < 0) {
        perror("select error");
    }

    if (FD_ISSET(server_fd, &temp_fds)) {
        new_socket = accept(server_fd, (struct sockaddr *)&server_address, (socklen_t*)&addr_len);
        if (new_socket < 0) {
            throw RuntimeError("Accept failed.");
        }

        FD_SET(new_socket, &read_fds);
        if (new_socket > max_fd) {
            max_fd = new_socket;
        }
    }

    for (int i = 0; i <= max_fd; i++) {
        if (FD_ISSET(i, &temp_fds)) {
            if (i != server_fd) {
                char buffer[1024];
                int valread = read(i, buffer, 1024);
                if (valread == 0) {
                    close(i);
                    FD_CLR(i, &read_fds);
                } else {
                    buffer[valread] = '\0';
					
                }
            }
        }
    }
}



}
	

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
		int server_fd;
		// Diğer üyeler...

	public:
		void arg_control(char **argv);
};

#endif // SERVER_HPP