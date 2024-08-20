#include "../includes/Channel.hpp"

Channel::Channel(const string& channelName, const string& channelKey, Client* channelOwner) : _channel_name(channelName), _channel_owner(channelOwner),
	  _channel_key(channelKey),
	  _user_limit(1000),
	  _no_external_messages(false),
	  _moderated(false)
{
}

Channel::~Channel()
{
}

void Channel::broadcastMessage(const string& message) const
{
	string tmp = message;
	for (vector<Client*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		(*it)->sendMessage(message);
	}
}

void Channel::broadcastMessage(const string& message, Client* exceptClient) const
{
	for (vector<Client*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (*it != exceptClient)
			(*it)->sendMessage(message);
	}
}

void Channel::addClient(Client* client)
{
	_clients.push_back(client);
}

void Channel::removeUserFromChannel(Client* client)
{
	for (vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (*it == client)
		{
			_clients.erase(it);
			break;
		}
	}
	if (client->isOperator() == true)
	{
		client->setOperator(false);
		for (vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
		{
			if ((*it)->isOperator() == true)
				return;
		}
		if (_clients.size() > 0)
		{
			Client* newOperator = _clients.front();
			if (newOperator->isOperator() == false)
			{
				newOperator->setOperator(true);
				set_channel_owner(newOperator);
				broadcastMessage("MODE " + _channel_name + " +o " + newOperator->getNickName());
			}
		}
	}
}

bool Channel::is_user_on_channel(Client* client) const
{
	for (vector<Client*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (*it == client)
			return true;
	}
	return false;
}

void Channel::setUpModeChannel(Channel* channel, Client* client, string& mode, string& modeParams)
{
	if (!channel || !client)
	{
		write(2, "Channel or Client is null, cannot set mode.", 43);
		return;
	}
	if (mode == "+k" && (client->isOperator() == true))
	{
		channel->set_channel_key(modeParams);
		channel->broadcastMessage("MODE " + channel->get_channel_name() + " +k " + client->getNickName());
	}
	else if (mode == "+l" && (client->isOperator() == true))
	{
		channel->set_user_limit(atoi(modeParams.c_str()));
		channel->broadcastMessage("MODE " + channel->get_channel_name() + " +l " + client->getNickName());
	}
	else if (mode == "+o" && (client->isOperator() == true))
	{
		Client* targetClient = Server::getInstance()->getClient(modeParams);
		if (!targetClient)
		{
			client->sendMessage(":" + client->getHostName() + " 401 " + client->getNickName() + " " + modeParams + " :No such nick/channel\r\n");
			return;
		}
		targetClient->setOperator(true);
		channel->broadcastMessage("MODE " + channel->get_channel_name() + " +o " + targetClient->getNickName());
	}
	else if (mode == "+m" && client->isOperator() == true)
	{
		channel->setModerated(true);
		channel->broadcastMessage("MODE " + channel->get_channel_name() + " +m " + client->getNickName());
	}
	else if (mode == "+n" && (client->isOperator() == true))
	{
		channel->set_no_external_messages(true);
		channel->broadcastMessage("MODE " + channel->get_channel_name() + " +n " + client->getNickName());
	}
	else
	{
		client->sendMessage(":" + client->getHostName() + " 501 " + client->getNickName() + " :Unknown MODE flag");
	}
}

void Channel::setLowModeChannel(Channel* channel, Client* client, string& mode, string& modeParams)
{
	if (!channel || !client)
	{
		write(2, "Channel or Client is not valid, cannot set mode.", 47);
		return;
	}
	if (mode == "-k" && (client->isOperator() == true))
	{
		channel->set_channel_key("");
		channel->broadcastMessage("MODE " + channel->get_channel_name() + " -k " + client->getNickName());
	}
	else if (mode == "-l" && (client->isOperator() == true))
	{
		channel->set_user_limit(1000);
		channel->broadcastMessage("MODE " + channel->get_channel_name() + " -l " + client->getNickName());
	}
	else if (mode == "-n" && (client->isOperator() == true))
	{
		channel->set_no_external_messages(false);
		channel->broadcastMessage("MODE " + channel->get_channel_name() + " -n " + client->getNickName());
	}
	else if (mode == "-o" && (client->isOperator() == true))
	{
		Client* targetClient = Server::getInstance()->getClient(modeParams);
		if (!targetClient)
		{
			client->sendMessage(":" + client->getHostName() + " 401 " + client->getNickName() + " " + modeParams + " :No such nick/channel\r\n");
			return;
		}
		if (targetClient->getNickName() != channel->get_channel_owner()->getNickName())
		{
			targetClient->setOperator(false);
			channel->broadcastMessage("MODE " + channel->get_channel_name() + " -o " + targetClient->getNickName());
		}
	}
	else if (mode == "-m" && client->isOperator() == true)
	{
		channel->setModerated(false);
		channel->broadcastMessage("MODE " + channel->get_channel_name() + " -m " + client->getNickName());
	}
	else
		client->sendMessage(":" + client->getHostName() + " 501 " + client->getNickName() + " :Unknown MODE flag");
}

std::vector<std::string> Channel::get_channel_client_nick_names() const
{
	vector<string> nickNames;
	for (vector<Client*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		nickNames.push_back((*it)->getNickName());
	}
	return nickNames;
}

string Channel::get_existing_users_nick_list() const
{
	string nickList;
	for (std::vector<Client*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (it != _clients.begin())
			nickList += " ";

		nickList += (*it)->getNickName();
	}
	return nickList;
}
