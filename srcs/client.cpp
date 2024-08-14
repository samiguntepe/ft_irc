#include "../includes/Client.hpp"

Client::Client(int client_socket_fd, int clientPort, const string& hostName, const string& serverName)
	: client_socket_fd(client_socket_fd),
	  _clientPort(clientPort),
	  _hostName(hostName),
	  server_name(serverName),
	  clientstatus(CLIENT_CONNECTED),
	  _channel(),
	  userAuth(false),
	  _isPass(false),
	  _isOperator(false)
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
	_clientPort = copy._clientPort;
	_nickName = copy._nickName;
	_userName = copy._userName;
	_realName = copy._realName;
	_hostName = copy._hostName;
	server_name = copy.server_name;
	clientstatus = copy.clientstatus;
	_channel = copy._channel;
	userAuth = copy.userAuth;
	_isPass = copy._isPass;
	_isOperator = copy._isOperator;
	return *this;
}

string Client::getPrefix() const
{
	string username = _userName.empty() ? "defUN" : _userName;
	string hostname = _hostName.empty() ? "defHN" : _hostName;
	return _nickName + "!" + username + "@" + hostname;
}

void Client::setNickName(const string& nickName)
{
	if (isValidName(nickName))
		_nickName = nickName;
	else
	{
		string error = "Invalid nickname: " + nickName;
		write(2, error.c_str(), error.length());
	}
}

void Client::setUserName(const string& userName)
{
	if (isValidName(userName))
		_userName = userName;
	else
	{
		string error = "Invalid username: " + userName;
		write(2, error.c_str(), error.length());
	}
}

void Client::welcomeMessage()
{
	if (clientstatus != CLIENT_CONNECTED || _nickName.empty() || _userName.empty() || _realName.empty())
	{
		sendReply("ERROR :Closing Link: " + _hostName + " (Invalid nickname or username)");
		return;
	}
	clientstatus = CLIENT_REGISTERED;
	sendReply(WELCOME_MESSAGE(server_name, _nickName));
	std::ostringstream oss;
	oss << _hostName << ":" << _clientPort << " is now known as " << _nickName << ".";
	log(oss.str());
}

void Client::sendMessage(const string& message) const
{
	string buffer = message + "\r\n";
	if (send(client_socket_fd, buffer.c_str(), buffer.length(), 0) == -1)
		ErrorLogger(FAILED_SOCKET_SEND, __FILE__, __LINE__);
}

void Client::sendReply(const string& reply) const
{
	sendMessage(getPrefix() + " " + reply);
}

void Client::leave()
{
	clientstatus = CLIENT_DISCONNECTED;
}

void Client::join(Channel* channel)
{
	channel->addClient(this);
	_channel.push_back(channel);
	std::string nickList;
	std::vector<std::string> nicknames = channel->getChannelClientNickNames();
	for (std::vector<std::string>::iterator it = nicknames.begin(); it != nicknames.end(); ++it)
	{
		nickList += *it + " ";
	}
	sendReply(RPL_NAMREPLY(getPrefix(), channel->getChannelName(), nickList));
	channel->broadcastMessage(RPL_JOIN(getPrefix(), channel->getChannelName()));
	string message = _nickName + " " + " has joined to the channel " + channel->getChannelName();
	log(message);
}

void Client::removeChannel(Channel* channel)
{
	std::vector<Channel*>::iterator it = std::find(_channel.begin(), _channel.end(), channel);
	if (it != _channel.end())
		_channel.erase(it);
}
