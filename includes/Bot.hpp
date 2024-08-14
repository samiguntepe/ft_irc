#pragma once

#include "Server.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include <unistd.h>
#include <string>

using namespace std;

class Server;
class Bot {
	private:
		int		sock;
		string	serv;
		int		port;
		string	botPass;
		string	nick;
		string	user;
		string	realname;

	public:
		Bot(const string &serv, int port, const string &pass);
		~Bot();

		string getBotnick() const { return nick; }
		void connectServ();
		void sendMsg(const string &channel, const string &msg);
		void sendRegMsg(const string &msg);
		void listen(Server *srv);
		void processMessage(const string &msg, Server *srv);
		int getSocket() const { return sock; }

		void WelcomeMsg(const string &userNick) {
			sendMsg(userNick, "Welcome to ft_irc Server");
		}
};

