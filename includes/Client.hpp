#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <algorithm>
#include <sys/socket.h>
#include "Server.hpp"
#include "Channel.hpp"
#include "Utilities.hpp"

class Server;
class Channel;

using namespace std;

enum ClientStatus
{
	CLIENT_HANDSHAKING,
	CLIENT_CONNECTED,
	CLIENT_REGISTERED,
	CLIENT_DISCONNECTED
};

class Client
{
private:
	int client_socket_fd;
	int _clientPort;

	string _nickName;
	string _userName;
	string _realName;
	string _hostName;
	string server_name;
	string buffer;
	ClientStatus clientstatus;
	::vector<Channel*> _channel;

	bool	userAuth;
	bool	_isPass;
	bool	_isOperator;
	bool isValidName( const string& name ) const
	{
		const int MAX_LENGTH = 15;
		if (name.length() > MAX_LENGTH) {
			return false;
		}
		for (std::string::const_iterator it = name.begin(); it != name.end(); ++it) {
			if (!isalnum(*it)) {
				return false;
			}
		}
		return true;
	}
public:
	Client(){};
	Client& operator=( const Client& copy );
	Client( const Client& copy ){ *this = copy;};
	Client( int client_socket_fd, int clientPort, const string& hostName, const string& serverName );
	~Client();

	int getclient_socket_fd() const { return client_socket_fd; }
	int getClientPort() const { return _clientPort; }
	bool isSocketOpen() const { return client_socket_fd != -1; }
	bool isOperator() const { return _isOperator; }
	bool getUserAuth() const { return userAuth; }
	bool getIsPass() const { return _isPass; }
	bool getValidName( const string& name ) const { return isValidName(name); }
	string getNickName() const { return _nickName; }
	string getUserName() const { return _userName; }
	string getRealName() const { return _realName; }
	string getHostName() const { return _hostName; }
	string getPrefix() const;
	string& getBuffer() { return buffer; }

	bool isRegistered() const { return clientstatus == CLIENT_REGISTERED; }

	vector<Channel*> getChannel() const { return _channel; }
	void appendtoBuffer( const string& message ) {  buffer.append(message); }
	void clearBuffer() { buffer.clear(); }
	void setPass( bool isPass) { _isPass = isPass; }
	void setNickName( const string& nickName );
	void setUserName( const string& userName );
	void setRealName( const string& realName ) { _realName = realName; }
	void setStatus( ClientStatus clientStatus ) { clientstatus = clientStatus; }
	void setChannel( Channel* channel ) { _channel.push_back(channel); }
	void setOperator( bool isOperator ) { _isOperator = isOperator; }
	void setUserAuth( bool isAuth ) { userAuth = isAuth; }
	void welcomeMessage();
	void sendMessage( const string& message ) const;
	void sendReply( const string& reply ) const;
	void leave();
	void join( Channel* channel );
	void removeChannel( Channel* channel );
};
