#ifndef BOT_HPP
#define BOT_HPP

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
		string	bot_password;
		string	nick;
		string	user;
		string	realname;

	public:
		Bot(const string &serv, int port, const string &pass);
		~Bot();

		void connect_server();
		void send_message(const string &channel, const string &msg);
		void send_re_message(const string &msg);
		void listen(Server *srv);
		void process_message(const string &msg, Server *srv);
		int getSocket() const { return sock; }

		void WelcomeMsg(const string &userNick) {
			send_message(userNick, "Welcome to ft_irc Server");
		}
};


#endif