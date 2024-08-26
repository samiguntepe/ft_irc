#include "../includes/Client.hpp"

Client::Client(int clientSocketFD, int clientPort, const string& hostName, const string& serverName)
	: _clientSocketFD(clientSocketFD),
	  _clientPort(clientPort),
	  _hostName(hostName),
	  _serverName(serverName),
	  _clientStatus(CLIENT_CONNECTED),
	  _channel(),
	  userAuth(false),
	  _isPass(false),
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
	_clientSocketFD = copy._clientSocketFD;
	_clientPort = copy._clientPort;
	_nickName = copy._nickName;
	_userName = copy._userName;
	_realName = copy._realName;
	_hostName = copy._hostName;
	_serverName = copy._serverName;
	_clientStatus = copy._clientStatus;
	_channel = copy._channel;
	userAuth = copy.userAuth;
	_isPass = copy._isPass;
	_is_operator = copy._is_operator;
	return *this;
}

string Client::get_prefix() const
{
	string username = _userName.empty() ? "defUN" : _userName;
	string hostname = _hostName.empty() ? "defHN" : _hostName;
	return _nickName + "!" + username + "@" + hostname;
}

void Client::set_nick_name(const string& nickName)
{
	if (isValidName(nickName))
		_nickName = nickName;
	else
	{
		string error = "Invalid nickname: " + nickName;
		write(2, error.c_str(), error.length());
	}
}

void Client::set_user_name(const string& userName)
{
	if (isValidName(userName))
		_userName = userName;
	else
	{
		string error = "Invalid username: " + userName;
		write(2, error.c_str(), error.length());
	}
}

void Client::welcome_message()
{
	if (_clientStatus != CLIENT_CONNECTED || _nickName.empty() || _userName.empty() || _realName.empty())
	{
		send_reply("ERROR :Closing Link: " + _hostName + " (Invalid nickname or username)");
		return;
	}
	_clientStatus = CLIENT_REGISTERED;
	send_reply(WELCOME_MESSAGE(_serverName, _nickName));
}

void Client::send_message(const string& message) const
{
	string buffer = message + "\r\n";
	if (send(_clientSocketFD, buffer.c_str(), buffer.length(), 0) == -1)
		ErrorLogger(FAILED_SOCKET_SEND, __FILE__, __LINE__);
}

void Client::send_reply(const string& reply) const
{
	send_message(get_prefix() + " " + reply);
}

void Client::leave()
{
	_clientStatus = CLIENT_DISCONNECTED;
}

void Client::join(Channel* channel)
{
	channel->add_client(this);
	_channel.push_back(channel);
	std::string nickList;
	std::vector<std::string> nicknames = channel->get_channel_client_nick_names();
	for (std::vector<std::string>::iterator it = nicknames.begin(); it != nicknames.end(); ++it)
	{
		nickList += *it + " ";
	}
	send_reply(RPL_NAMREPLY(get_prefix(), channel->get_channel_name(), nickList));
	channel->broadcast_message(RPL_JOIN(get_prefix(), channel->get_channel_name()));
}

void Client::remove_channel(Channel* channel)
{
	std::vector<Channel*>::iterator it = std::find(_channel.begin(), _channel.end(), channel);
	if (it != _channel.end())
		_channel.erase(it);
}
