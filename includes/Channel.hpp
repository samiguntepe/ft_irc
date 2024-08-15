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

		string	_channelKey;
		int		_userLimit;
		bool	_noExternalMessages;
		bool	_moderated;

		Channel();
		Channel( const Channel& copy );


	public:
		Channel ( const string& channel_name, const string& channelKey, Client* channelOwner );
		~Channel();

		string getchannel_name() const { return _channel_name; }
		Client* getChannelOwner() const { return _channelOwner; }

		string getChannelKey() const { return _channelKey; }
		bool isUserOnChannel( Client* client ) const;
		int getUserLimit() const { return _userLimit; }
		int getChannelClientCount() const { return clients.size(); }
		bool getNoExternalMessages() const { return _noExternalMessages; }
		vector<string> getChannelClientNickNames() const;
		string getExistingUsersNickList() const;
		bool getModerated() const { return _moderated; }

		void setChannelOwner( Client* client ){ _channelOwner = client ;};
		void setChannelKey( const string& chanelKey ) { _channelKey = chanelKey; }
		void setUserLimit( int userLimit ) { _userLimit = userLimit; }
		void setNoExternalMessages( bool noExternalMessages ) { _noExternalMessages = noExternalMessages; }

		void setModerated( bool isOwner ) { _moderated = isOwner; }
		void broadcastMessage( const string& message ) const;
		void broadcastMessage( const string& message, Client* exceptClient ) const;

		void addClient( Client* client );
		void removeUserFromChannel( Client* client );
		void kickClient( Client* client, Client* target, const string& reason = "" );
		void setUpModeChannel(Channel* channel, Client* client, string& mode, string& modeParams);
		void setLowModeChannel(Channel* channel, Client* client, string& mode, string& modeParams);
};

#endif