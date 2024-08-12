#include "../includes/Client.hpp"
#include "../includes/Server.hpp"

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
		// cli_buffer = cli_buffer + _cli_buffer;
		cli_buffer.append(_cli_buffer);
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

void Client::send_message(const std::string& message)
{
	Client client;

	std::string buffer = message + "\r\n";
	if (send(client.cli_fd, buffer.c_str(), buffer.length(), 0) == -1)
		RuntimeError("Failed to send socket");
}