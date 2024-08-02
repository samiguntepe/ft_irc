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
	this->socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (this->socket_fd < 0)
		throw RuntimeError("Socket creation failed.");

	std::memset(&socket_feature, 0, sizeof(socket_feature));
	socket_feature.sin_family = AF_INET;
	socket_feature.sin_port = htons(port_number);
	socket_feature.sin_addr.s_addr = inet_addr("127.0.0.1");
}

void Server::bind_listen_socket()
{
	if (bind(this->socket_fd, (struct sockaddr *)&socket_feature, sizeof(socket_feature)) < 0)
	{
		close(this->socket_fd);
		throw RuntimeError("Bind failed.");
	}
	if (listen(this->socket_fd, 42) < 0)
	{
		close(this->socket_fd);
		throw RuntimeError("Listen failed.");
	}
	std::cout << "Waiting for connections..." << std::endl;
}

void Server::accept_select_socket()
{
	sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);
	char buff[1024];
	fd_set read_fds;
	std::vector<int> client_fds;

	while (true)
	{
		int client_fd = accept(socket_fd, (struct sockaddr *)&client_addr, &client_len);
		if (client_fd < 0)
		{
			close(socket_fd);
			throw RuntimeError("Accept failed.");
		}

		std::cout << "\nClient number " << client_fd << " connected." << std::endl;
		client_fds.push_back(client_fd);

		while (true)
		{
			FD_ZERO(&read_fds);
			FD_SET(socket_fd, &read_fds);
			int max_fd = socket_fd;

			for (std::vector<int>::iterator it = client_fds.begin(); it != client_fds.end(); ++it)
			{
				int fd = *it;
				FD_SET(fd, &read_fds);
				if (fd > max_fd)
				{
					max_fd = fd;
				}
			}

			int activity = select(max_fd + 1, &read_fds, 0, 0, 0);

			if (activity < 0)
				throw RuntimeError("Select failed.");

			if (FD_ISSET(socket_fd, &read_fds))
			{
				int new_client_fd = accept(socket_fd, (struct sockaddr *)&client_addr, &client_len);
				if (new_client_fd < 0)
				{
					std::cout << "Accept failed." << std::endl;
					continue;
				}
				std::cout << "\nClient number " << new_client_fd << " connected.\n" << std::endl;
				client_fds.push_back(new_client_fd);
			}

			for (std::vector<int>::iterator it = client_fds.begin(); it != client_fds.end();)
			{
				int fd = *it;
				if (FD_ISSET(fd, &read_fds))
				{
					ssize_t bytes_received = recv(fd, buff, sizeof(buff), 0);
					if (bytes_received > 0)
					{
						buff[bytes_received] = '\0';
						std::cout << "Client number " << *it << " message: " << buff << std::endl;
					}
					else
					{
						if (bytes_received == 0)
						{
							std::cout << "Client number " << *it << " disconnected\n" << std::endl;
						}
						else
						{
							std::cout << "Receive failed." << std::endl;
						}
						close(fd);
						it = client_fds.erase(it);
						continue;
					}
				}
				++it;
			}
		}
	}
	close(this->socket_fd);
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
		int socket_fd;
		// Diğer üyeler...

	public:
		void arg_control(char **argv);
};

#endif // SERVER_HPP