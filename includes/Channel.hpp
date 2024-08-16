#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <unistd.h>
#include <sstream>
#include <vector>
#include "Server.hpp"
#include "Bot.hpp"
#include "Client.hpp"

class Client;

using namespace std;

class Channel
{
	private:
		string _channel_name;
		vector<Client*> clients;
		Client* _channelOwner;

		string	_channel_key;
		int		_user_limit;
		bool	_no_external_messages;
		bool	_moderated;

		Channel();
		Channel( const Channel& copy );


	public:
		Channel ( const string& channel_name, const string& channelKey, Client* channelOwner );
		~Channel();

		string get_channel_name() const { return _channel_name; }
		Client* get_channel_owner() const { return _channelOwner; }

		string get_channel_key() const { return _channel_key; }
		bool is_user_on_channel( Client* client ) const;
		int get_user_limit() const { return _user_limit; }
		int get_channel_client_count() const { return clients.size(); }
		bool get_no_external_messages() const { return _no_external_messages; }
		vector<string> get_Channel_Client_Nick_Names() const;
		string get_existing_users_nick_list() const;
		bool get_moderated() const { return _moderated; }

		void set_channel_owner( Client* client ){ _channelOwner = client ;};
		void set_channel_key( const string& chanelKey ) { _channel_key = chanelKey; }
		void set_user_limit( int userLimit ) { _user_limit = userLimit; }
		void set_no_external_messages( bool noExternalMessages ) { _no_external_messages = noExternalMessages; }

		void set_moderated( bool isOwner ) { _moderated = isOwner; }
		void broadcast_message( const string& message ) const;
		void broadcast_message( const string& message, Client* exceptClient ) const;

		void add_client( Client* client );
		void remove_user_from_channel( Client* client );
		void set_up_mode_channel(Channel* channel, Client* client, string& mode, string& modeParams);
		void set_low_mode_channel(Channel* channel, Client* client, string& mode, string& modeParams);
};

#endif