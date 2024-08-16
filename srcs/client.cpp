#include "../includes/Client.hpp"

Client::Client(int client_socket_fd, int clientPort, const string& hostName, const string& server_name)
	: client_socket_fd(client_socket_fd),
	  _client_port(clientPort),
	  _host_name(hostName),
	  server_name(server_name),
	  clientstatus(CLIENT_CONNECTED),
	  _channel(),
	  user_auth(false),
	  _is_password(false),
	  _is_operator(false)
{
}

Client::~Client()
{
	for (std::vector<Channel*>::iterator it = _channel.begin(); it != _channel.end(); ++it)
	{
		delete *it;
		*it = NULL;
	}
	_channel.clear();
}

Client& Client::operator=(const Client& copy)
{
	client_socket_fd = copy.client_socket_fd;
	_client_port = copy._client_port;
	_nick_name = copy._nick_name;
	_user_name = copy._user_name;
	_real_name = copy._real_name;
	_host_name = copy._host_name;
	server_name = copy.server_name;
	clientstatus = copy.clientstatus;
	_channel = copy._channel;
	user_auth = copy.user_auth;
	_is_password = copy._is_password;
	_is_operator = copy._is_operator;
	return *this;
}

string Client::get_prefix() const
{
	string username = _user_name.empty() ? "defUN" : _user_name;
	string hostname = _host_name.empty() ? "defHN" : _host_name;
	return _nick_name + "!" + username + "@" + hostname;
}

void Client::set_nick_name(const string& nickName)
{
	if (isValidName(nickName))
		_nick_name = nickName;
	else
	{
		string error = "Invalid nickname: " + nickName;
		write(2, error.c_str(), error.length());
	}
}

void Client::set_user_name(const string& userName)
{
	if (isValidName(userName))
		_user_name = userName;
	else
	{
		string error = "Invalid username: " + userName;
		write(2, error.c_str(), error.length());
	}
}

void Client::welcome_message()
{
	if (clientstatus != CLIENT_CONNECTED || _nick_name.empty() || _user_name.empty() || _real_name.empty())
	{
		send_reply("ERROR :Closing Link: " + _host_name + " (Invalid nickname or username)");
		return;
	}
	clientstatus = CLIENT_REGISTERED;
	send_reply(WELCOME_MESSAGE(server_name, _nick_name));
	std::ostringstream oss;
	oss << _host_name << ":" << _client_port << " is now known as " << _nick_name << ".";
	log(oss.str());
}

void Client::send_message(const string& message) const
{
	string buffer = message + "\r\n";
	if (send(client_socket_fd, buffer.c_str(), buffer.length(), 0) == -1)
		ErrorLogger(FAILED_SOCKET_SEND, __FILE__, __LINE__);
}

void Client::send_reply(const string& reply) const
{
	send_message(get_prefix() + " " + reply);
}

void Client::leave()
{
	clientstatus = CLIENT_DISCONNECTED;
}

void Client::join(Channel* channel)
{
	channel->add_client(this);
	_channel.push_back(channel);
	std::string nickList;
	std::vector<std::string> nicknames = channel->get_Channel_Client_Nick_Names();
	for (std::vector<std::string>::iterator it = nicknames.begin(); it != nicknames.end(); ++it)
	{
		nickList += *it + " ";
	}
	send_reply(RPL_NAMREPLY(get_prefix(), channel->get_channel_name(), nickList));
	channel->broadcast_message(RPL_JOIN(get_prefix(), channel->get_channel_name()));
	string message = _nick_name + " " + " has joined to the channel " + channel->get_channel_name();
	log(message);
}

void Client::remove_channel(Channel* channel)
{
	std::vector<Channel*>::iterator it = std::find(_channel.begin(), _channel.end(), channel);
	if (it != _channel.end())
		_channel.erase(it);
}
