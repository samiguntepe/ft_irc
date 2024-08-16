#include "../includes/Channel.hpp"

Channel::Channel(const string& channel_name, const string& channelKey, Client* channelOwner) : _channel_name(channel_name), _channelOwner(channelOwner),
	  _channel_key(channelKey),
	  _user_limit(1000),
	  _no_external_messages(false),
	  _moderated(false)
{
}

Channel::~Channel()
{
}

void Channel::broadcast_message(const string& message) const
{
	string tmp = message;
	for (vector<Client*>::const_iterator it = clients.begin(); it != clients.end(); ++it)
	{
		(*it)->send_message(message);
	}
}

void Channel::broadcast_message(const string& message, Client* exceptClient) const
{
	for (vector<Client*>::const_iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if (*it != exceptClient)
			(*it)->send_message(message);
	}
}

void Channel::add_client(Client* client)
{
	clients.push_back(client);
}

void Channel::remove_user_from_channel(Client* client)
{
	for (vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if (*it == client)
		{
			clients.erase(it);
			break;
		}
	}
	if (client->is_operator() == true)
	{
		client->set_operator(false);
		for (vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it)
		{
			if ((*it)->is_operator() == true)
				return;
		}
		if (clients.size() > 0)
		{
			Client* newOperator = clients.front();
			if (newOperator->is_operator() == false)
			{
				newOperator->set_operator(true);
				set_channel_owner(newOperator);
				broadcast_message("MODE " + _channel_name + " +o " + newOperator->get_nick_name());
			}
		}
	}
}

bool Channel::is_user_on_channel(Client* client) const
{
	for (vector<Client*>::const_iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if (*it == client)
			return true;
	}
	return false;
}

void Channel::set_up_mode_channel(Channel* channel, Client* client, string& mode, string& modeParams)
{
	if (!channel || !client)
	{
		write(2, "Channel or Client is null, cannot set mode.", 43);
		return;
	}
	if (mode == "+k" && (client->is_operator() == true))
	{
		channel->set_channel_key(modeParams);
		channel->broadcast_message("MODE " + channel->get_channel_name() + " +k " + client->get_nick_name());
	}
	else if (mode == "+l" && (client->is_operator() == true))
	{
		channel->set_user_limit(atoi(modeParams.c_str()));
		channel->broadcast_message("MODE " + channel->get_channel_name() + " +l " + client->get_nick_name());
	}
	else if (mode == "+o" && (client->is_operator() == true))
	{
		Client* target_client = Server::get_instance()->get_client(modeParams);
		if (!target_client)
		{
			client->send_message(":" + client->get_host_name() + " 401 " + client->get_nick_name() + " " + modeParams + " :No such nick/channel\r\n");
			return;
		}
		target_client->set_operator(true);
		channel->broadcast_message("MODE " + channel->get_channel_name() + " +o " + target_client->get_nick_name());
	}
	else if (mode == "+m" && client->is_operator() == true)
	{
		channel->set_moderated(true);
		channel->broadcast_message("MODE " + channel->get_channel_name() + " +m " + client->get_nick_name());
	}
	else if (mode == "+n" && (client->is_operator() == true))
	{
		channel->set_no_external_messages(true);
		channel->broadcast_message("MODE " + channel->get_channel_name() + " +n " + client->get_nick_name());
	}
	else
	{
		client->send_message(":" + client->get_host_name() + " 501 " + client->get_nick_name() + " :Unknown MODE flag");
	}
}

void Channel::set_low_mode_channel(Channel* channel, Client* client, string& mode, string& modeParams)
{
	if (!channel || !client)
	{
		write(2, "Channel or Client is not valid, cannot set mode.", 47);
		return;
	}
	if (mode == "-k" && (client->is_operator() == true))
	{
		channel->set_channel_key("");
		channel->broadcast_message("MODE " + channel->get_channel_name() + " -k " + client->get_nick_name());
	}
	else if (mode == "-l" && (client->is_operator() == true))
	{
		channel->set_user_limit(1000);
		channel->broadcast_message("MODE " + channel->get_channel_name() + " -l " + client->get_nick_name());
	}
	else if (mode == "-n" && (client->is_operator() == true))
	{
		channel->set_no_external_messages(false);
		channel->broadcast_message("MODE " + channel->get_channel_name() + " -n " + client->get_nick_name());
	}
	else if (mode == "-o" && (client->is_operator() == true))
	{
		Client* target_client = Server::get_instance()->get_client(modeParams);
		if (!target_client)
		{
			client->send_message(":" + client->get_host_name() + " 401 " + client->get_nick_name() + " " + modeParams + " :No such nick/channel\r\n");
			return;
		}
		if (target_client->get_nick_name() != channel->get_channel_owner()->get_nick_name())
		{
			target_client->set_operator(false);
			channel->broadcast_message("MODE " + channel->get_channel_name() + " -o " + target_client->get_nick_name());
		}
	}
	else if (mode == "-m" && client->is_operator() == true)
	{
		channel->set_moderated(false);
		channel->broadcast_message("MODE " + channel->get_channel_name() + " -m " + client->get_nick_name());
	}
	else
		client->send_message(":" + client->get_host_name() + " 501 " + client->get_nick_name() + " :Unknown MODE flag");
}

std::vector<std::string> Channel::get_Channel_Client_Nick_Names() const
{
	vector<string> nickNames;
	for (vector<Client*>::const_iterator it = clients.begin(); it != clients.end(); ++it)
	{
		nickNames.push_back((*it)->get_nick_name());
	}
	return nickNames;
}

string Channel::get_existing_users_nick_list() const
{
	string nickList;
	for (std::vector<Client*>::const_iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if (it != clients.begin())
			nickList += " ";

		nickList += (*it)->get_nick_name();
	}
	return nickList;
}
