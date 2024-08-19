#pragma once

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

		void socketStart();
		void socketInit();
		void socketBind();
		void socketListen();
		int socketAccept();

	public:
		Server();
		Server ( int serverSocketFamily, int serverSocketProtocol, int serverSocketPort, string serverName );
		~Server();

		map<int, Client*> getAllClients() {
		return _clients;
		}
		void serverRun();
		void shutdownSrv();

		static void signalHandler(int signum);
		static void signalHandlerServer(int signum);
		void handleClient(int clientSocketFD);
		static Server* getInstance() {return ins;}
		static void setInstance(Server* server){ins = server;}
		Client* getClient( string& nickName );
		void removeClientFromAllChannels( Client* client );
		void clientDisconnect(int clientSocketFD);
		void setSrvPass(const string& pass);
		string getSrvPass() const{return _serverPass;}
		void addChannel( Channel* channel );
		Channel* getChannel( string& channelName );
		bool channelExists( const string& channelName );
		bool verifySrvPass(const string& pass);
		void removeChannel(const string& channel );
		void processPartialCommands(int clientSocketFD);
		Bot* getBot() { return _bot; }
};
