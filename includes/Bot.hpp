#ifndef BOT_HPP
#define BOT_HPP

#include "Server.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include <unistd.h>
#include <string>

class Server;
class Bot {
	private:
		int		sock;
		std::string	serv;
		int		port;
		std::string	bot_password;
		std::string	nick;
		std::string	user;
		std::string	realname;

	public:
		Bot(const std::string &serv, int port, const std::string &pass);
		~Bot();

		std::string get_botnick() const { return nick; }
		void connect_server();
		void send_message(const std::string &channel, const std::string &msg);
		void send_re_message(const std::string &msg);
		void listen(Server *srv);
		void process_message(const std::string &msg, Server *srv);
		int get_socket() const { return sock; }

		void welcome_message(const std::string &userNick) {
			send_message(userNick, "Welcome to ft_irc Server");
		}
};

#endif