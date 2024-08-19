#include "../includes/Bot.hpp"
#include "../includes/Client.hpp"
#include "../includes/Server.hpp"

Bot::Bot(const string &serv, int port, const string &pass) : serv(serv), port(port), bot_password(pass), nick("reepNaoBot"), user("IRC00"), realname("recep")
{
	connect_server();
}

Bot::~Bot()
{
	close(sock);
}

void Bot::connect_server()
{
	struct sockaddr_in server_addr;
	struct hostent *host;
	host = gethostbyname(serv.c_str());
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		write(STDOUT_FILENO, "Error: Bot socket creation failed.\n", 34);
		exit(1);
	}
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr = *((struct in_addr *)host->h_addr);
	memset(&(server_addr.sin_zero), '\0', 8);
	if (connect(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
	{
		write(STDOUT_FILENO, "Error: Bot socket connection failed.\n", 36);
		exit(1);
	}
	if (fcntl(sock, F_SETFL, O_NONBLOCK) == -1)
	{
		write(STDOUT_FILENO, "Error: Bot socket fcntl failed.\n", 32);
		exit(1);
	}
	if (sock != -1)
	{
		send_re_message("PASS " + bot_password + "\r\n");
		send_re_message("NICK " + nick + "\r\n");
		send_re_message("USER " + user + " 0 * :" + realname + "\r\n");
	}
}

void Bot::listen(Server *srv)
{
	const int BUFFER_SIZE = 1024;
	char buffer[BUFFER_SIZE];
	memset(buffer, 0, BUFFER_SIZE);
	string accumulated;
	int bytes_read = recv(sock, buffer, BUFFER_SIZE - 1, 0);
	if (bytes_read < 0)
	{
		string errrecv = "Error in recv: " + string(strerror(errno)) + "\n";
		write(STDOUT_FILENO, errrecv.c_str(), errrecv.length());
		return;
	}
	else if (bytes_read == 0)
	{
		write(STDOUT_FILENO, "Connection closed by peer.\n", 27);
		return;
	}
	buffer[bytes_read] = '\0';
	accumulated += buffer;
	size_t pos;
	while ((pos = accumulated.find("\r\n")) != string::npos)
	{
		string msg = accumulated.substr(0, pos);
		accumulated.erase(0, pos + 2);
		process_message(msg, srv);
	}
}

void Bot::process_message(const string &msg, Server *srv)
{
	if (msg.find("PRIVMSG") != string::npos)
	{
		string senderNick = msg.substr(1, msg.find("!") - 1);
		{
			if (msg.find("hello") != string::npos)
				send_message(senderNick, "Hello " + senderNick + "!");
			if (msg.find("date") != string::npos)
			{
				time_t rawtime;
				struct tm *timeinfo;
				time(&rawtime);
				timeinfo = localtime(&rawtime);
				send_message(senderNick, asctime(timeinfo));
			}
			if (msg.find("badword") != string::npos || msg.find("swearword") != string::npos)
				send_message(senderNick, "Language please " + senderNick + "!");
			if (msg.find("whoami") != string::npos)
			{
				Client *client = srv->getClient(senderNick);
				send_message(senderNick,"My nickname->"+ client->getNickName());
				send_message(senderNick,"My realname->"+ client->getRealName());
				send_message(senderNick,"My hostname->"+ client->getHostName());
			}
		}
	}
}

void Bot::send_message(const string &channel, const string &message)
{
	string fullMessage = "PRIVMSG " + channel + " :" + message + "\r\n";
	send(sock, fullMessage.c_str(), fullMessage.length(), 0);
}

void Bot::send_re_message(const string &message)
{
	send(sock, message.c_str(), message.length(), 0);
}
