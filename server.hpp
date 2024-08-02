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

class RuntimeError : public std::exception
{
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
		int port_number;
		sockaddr_in socket_feature;
	public:
		void arg_control(char **argv);
		void create_socket();
		void bind_listen_socket();
		void accept_select_socket();
		void run();
};

#endif