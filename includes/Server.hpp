#ifndef SERVER_HPP
#define SERVER_HPP

#include <map>
#include <iostream>
#include <netdb.h>
#include <fcntl.h>
#include <stdio.h>
#include <csignal>
#include <sstream>
#include <unistd.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <exception>
#include "Bot.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Commands.hpp"
#include "Utilities.hpp"

#define RED	"\033[0;31m"
#define CODE "\033[m"


class Client;
class Channel;
class Bot;

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
		int server_fd;
		const int server_socket_family;
		const int server_socket_protocol;
		int port_number;
		std::string server_name;
		std::string	password;

		static Server *ins;
		struct sockaddr_in server_address;
		fd_set read_set;

		map<int, Client> client_buffers;
		map<int, Client*> clients;
		map<std::string, Channel*> _channels;
		Bot* _bot;

		void socket_start();
		void socket_init();
		void socket_bind();
		void socket_listen();
		int socket_accept();

	public:
		Server();
		Server ( int server_socket_family, int server_socket_protocol, string server_name );
		~Server();

		map<int, Client*> get_all_clients() {
		return clients;
		}
		void start();
		void shut_down_server();

		static void signal_handler(int signum);
		static void signal_handler_server(int signum);
		void handle_client(int client_socket_fd);
		static Server* get_instance() {return ins;}
		static void set_instance(Server* server){ins = server;}
		Client* get_client( string& nickName );
		void remove_client_all_channels( Client* client );
		void clientDisconnect(int client_socket_fd);
		void setSrvPass(const string& pass);
		string get_server_password() const{return password;};
		void addChannel( Channel* channel );
		Channel* get_channel( string& channel_name );
		bool channel_exists( const string& channel_name );
		bool verify_server_password(const string& pass);
		void remove_channel(const string& channel );
		void process_partial_commands(int client_socket_fd);
		void arg_control(char **);
		Bot* get_bot() { return _bot; }
};

#endif