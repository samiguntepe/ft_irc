#include "../includes/Client.hpp"

Client::Client(int fd)
{
	cli_fd = fd;
}

int Client::get_cli_fd() const
{
	return cli_fd;
}

void Client::set_cli_fd(int _cli_fd)
{
	cli_fd = _cli_fd;
}

void Client::set_cli_buffer(std::string _cli_buffer)
{
	if (cli_buffer == "")
	{
		cli_buffer = _cli_buffer;
	} else {
		cli_buffer.append(_cli_buffer);
	}
}

std::string Client::get_cli_buffer()
{
	return cli_buffer;
}

std::string Client::get_cli_nick()
{
	return cli_nick;
}

void Client::set_cli_nick(std::string _cli_nick)
{
	cli_nick = _cli_nick;
}