#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Server.hpp"
#include <iostream>
#include <vector>

class Client
{
 private:
	int cli_fd;
	std::string cli_nick;
	std::string cli_msg;
	std::string cli_buffer;
	std::vector<Channel*> channels;

 public:
	Client(){};
	Client(int fd);
	int get_cli_fd () const;
	void set_cli_fd ( int _cli_fd );
	void set_cli_buffer ( std::string );
	std::string get_cli_buffer ();
	std::string get_cli_nick ();
	void set_cli_nick ( std::string);
	static void send_message(const std::string& message);

};

#endif