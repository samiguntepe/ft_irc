#include "../includes/Server.hpp"
#include "../includes/Channel.hpp"

void Server::start()
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
    fd_set temp_read_fds = read_fds;

    int activity = select(max_fd + 1, &temp_read_fds, NULL, NULL, NULL);

    if (activity < 0) {
		throw RuntimeError("Select is failed.");
    }

    if (FD_ISSET(server_fd, &temp_read_fds)) {
        new_socket = accept(server_fd, (struct sockaddr *)&server_address, (socklen_t*)&addr_len);
		new_client(new_socket);
		std::cout << "New connection, socket fd is " << new_socket << "\nIP Address: " << inet_ntoa(server_address.sin_addr) << std::endl;

        if (new_socket < 0) {
            throw RuntimeError("Accept is failed.");
        }

        FD_SET(new_socket, &read_fds);
        if (new_socket > max_fd) {
            max_fd = new_socket;
        }
    }

	for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); )
	{
	  	int fd = (*it)->get_cli_fd();
	  	if (FD_ISSET(fd, &temp_read_fds)) {
	  	    if (fd != server_fd) {
			char buffer[1024] = {0};
	  	      int bytes_received = recv(fd, buffer, sizeof(buffer), 0);
	  	        if (bytes_received > 0) {

					buffer[bytes_received] = '\0';
					(*it)->set_cli_buffer(buffer);
					std::cout << "Received: " << buffer << std::endl; // veriler burada işlenmeli
	  	            ++it;
				
	  	        } else if (bytes_received == 0) {
					std::cout << "Connection closed by client" << std::endl;
	  	            close(fd);
	  	            FD_CLR(fd, &read_fds);
	  	            delete *it;
	  	            it = clients.erase(it);
	  	        } else{
					//hata;
				}
	  	    } else {
	  	        ++it;
	  	    }
	  	} else {
	  	    ++it;
	  		}
		}
	}
}

void Server::new_client(int fd)
{
    clients.push_back(new Client(fd));
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