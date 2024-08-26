#ifndef CLIENT_HPP
#define CLIENT_HPP

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
	int _clientSocketFD;
	int _clientPort;

	string _nickName;
	string _userName;
	string _realName;
	string _hostName;
	string _serverName;
	string buffer;
	ClientStatus _clientStatus;
	::vector<Channel*> _channel;

	bool	userAuth;
	bool	_isPass;
	bool	_is_operator;
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
	Client( int clientSocketFD, int clientPort, const string& hostName, const string& serverName );
	~Client();

	int get_client_socket_fd() const { return _clientSocketFD; }
	bool is_socket_open() const { return _clientSocketFD != -1; }
	bool is_operator() const { return _is_operator; }
	bool get_user_auth() const { return userAuth; }
	bool get_is_pass() const { return _isPass; }
	bool get_valid_name( const string& name ) const { return isValidName(name); }
	string get_nick_name() const { return _nickName; }
	string get_user_name() const { return _userName; }
	string get_real_name() const { return _realName; }
	string get_host_name() const { return _hostName; }
	string get_prefix() const;
	string& get_buffer() { return buffer; }

	bool is_registered() const { return _clientStatus == CLIENT_REGISTERED; }

	vector<Channel*> get_channel() const { return _channel; }
	void append_to_buffer( const string& message ) {  buffer.append(message); }
	void set_pass( bool isPass) { _isPass = isPass; }
	void set_nick_name( const string& nickName );
	void set_user_name( const string& userName );
	void set_real_name( const string& realName ) { _realName = realName; }
	void set_status( ClientStatus clientStatus ) { _clientStatus = clientStatus; }
	void set_channel( Channel* channel ) { _channel.push_back(channel); }
	void set_operator( bool is_operator ) { _is_operator = is_operator; }
	void set_user_auth( bool isAuth ) { userAuth = isAuth; }
	void welcome_message();
	void send_message( const string& message ) const;
	void send_reply( const string& reply ) const;
	void leave();
	void join( Channel* channel );
	void remove_channel( Channel* channel );
};

#endif