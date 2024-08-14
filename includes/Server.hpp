#pragma once

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
		struct sockaddr_in serverAddress;
		fd_set read_set;

		map<int, Client> clientBuffers;
		map<int, Client*> clients;
		map<std::string, Channel*> _channels;
		Bot* _bot;

		void socketStart();
		void socket_init();
		void socket_bind();
		void socket_listen();
		int socket_accept();

	public:
		Server();
		Server ( int serverSocketFamily, int serverSocketProtocol, string serverName );
		~Server();

		map<int, Client*> getAllClients() {
		return clients;
		}
		void start();
		void shutdownSrv();

		static void signalHandler(int signum);
		static void signalHandlerServer(int signum);
		void handleClient(int client_socket_fd);
		static Server* getInstance() {return ins;}
		static void setInstance(Server* server){ins = server;}
		Client* getClient( string& nickName );
		void removeClientFromAllChannels( Client* client );
		void clientDisconnect(int client_socket_fd);
		void setSrvPass(const string& pass);
		string get_server_password() const{return password;};
		void addChannel( Channel* channel );
		Channel* getChannel( string& channelName );
		bool channelExists( const string& channelName );
		bool verifySrvPass(const string& pass);
		void removeChannel(const string& channel );
		void processPartialCommands(int client_socket_fd);
		void arg_control(char **);
		Bot* getBot() { return _bot; }
};
