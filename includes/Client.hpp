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
	int client_socket_fd;
	int _client_port;

	string _nick_name;
	string _user_name;
	string _real_name;
	string _host_name;
	string server_name;
	string buffer;
	ClientStatus clientstatus;
	::vector<Channel*> _channel;

	bool	user_auth;
	bool	_is_password;
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
	Client( int client_socket_fd, int clientPort, const string& hostName, const string& server_name );
	~Client();

	int get_client_socket_fd() const { return client_socket_fd; }
	int get_client_port() const { return _client_port; }
	bool is_socket_open() const { return client_socket_fd != -1; }
	bool is_operator() const { return _is_operator; }
	bool get_user_auth() const { return user_auth; }
	bool get_is_pass() const { return _is_password; }
	bool get_valid_name( const string& name ) const { return isValidName(name); }
	string get_nick_name() const { return _nick_name; }
	string get_user_name() const { return _user_name; }
	string get_real_name() const { return _real_name; }
	string get_host_name() const { return _host_name; }
	string get_prefix() const;
	string& get_buffer() { return buffer; }

	bool is_registered() const { return clientstatus == CLIENT_REGISTERED; }

	vector<Channel*> get_channel() const { return _channel; }
	void append_to_buffer( const string& message ) {  buffer.append(message); }
	void clear_buffer() { buffer.clear(); }
	void set_password( bool isPass) { _is_password = isPass; }
	void set_nick_name( const string& nickName );
	void set_user_name( const string& userName );
	void set_real_name( const string& realName ) { _real_name = realName; }
	void set_status( ClientStatus clientStatus ) { clientstatus = clientStatus; }
	void set_channel( Channel* channel ) { _channel.push_back(channel); }
	void set_operator( bool is_operator ) { _is_operator = is_operator; }
	void set_user_auth( bool isAuth ) { user_auth = isAuth; }
	void welcome_message();
	void send_message( const string& message ) const;
	void send_reply( const string& reply ) const;
	void leave();
	void join( Channel* channel );
	void remove_channel( Channel* channel );
};

#endif