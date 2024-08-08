#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>

class Client
{
 private:
	int cli_fd;
	std::string cli_nick;
	std::string cli_msg;

 public:

	int get_cli_fd () const { return cli_fd; };
	void set_cli_fd ( int _cli_fd ) { cli_fd = _cli_fd; };

};

#endif