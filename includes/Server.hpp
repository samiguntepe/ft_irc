#ifndef SERVER_HPP
#define SERVER_HPP

#include <map>
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

using namespace std;

class Server
{
	private:
		int _serverSocketFD;
		const int _serverSocketFamily;
		const int _serverSocketProtocol;
		const int _serverSocketPort;
		string _serverName;
		string	_serverPass;

		static Server *ins;
		struct sockaddr_in serverAddress;
		fd_set read_set;

		map<int, Client> clientBuffers;
		map<int, Client*> _clients;
		map<string, Channel*> _channels;
		Bot* _bot;

		void socket_start();
		void socket_init();
		void socket_bind();
		void socket_listen();
		int socket_accept();

	public:
		Server();
		Server ( int serverSocketFamily, int serverSocketProtocol, int serverSocketPort, string serverName );
		~Server();

		map<int, Client*> get_all_clients() {
		return _clients;
		}
		void server_run();
		void shutdown_server();

		static void signal_handler(int signum);
		void handle_client(int clientSocketFD);
		static Server* get_instance() {return ins;}
		static void set_instance(Server* server){ins = server;}
		Client* get_client( string& nickName );
		void remove_client_from_all_channels( Client* client );
		void client_disconnect(int clientSocketFD);
		void set_server_password(const string& pass);
		string get_server_password() const{return _serverPass;}
		void add_channel( Channel* channel );
		Channel* get_channel( string& channelName );
		bool channel_exists( const string& channelName );
		bool verify_server_password(const string& pass);
		void remove_channel(const string& channel );
		void process_partial_commands(int clientSocketFD);
		Bot* get_bot() { return _bot; }
};

#endif