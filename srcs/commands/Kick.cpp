#include "../../includes/Commands.hpp"

void Kick::kick(Client* client, vector<string> commandParts, Server* srv)
{
    if (commandParts.size() < 3)
    {
        client->send_reply(ERR_NEEDMOREPARAMS(client->get_nick_name(), "KICK"));
        return;
    }
    string channel_namee = commandParts.at(1);
    string nickName = commandParts.at(2);
    string reason = (commandParts.size() > 3) ? commandParts.at(3) : "No reason specified";
    if (channel_namee.at(0) != '#')
    {
        client->send_reply(ERR_NOSUCHCHANNEL(client->get_nick_name(), channel_namee));
        return;
    }
    Channel* channel = srv->get_channel(channel_namee);
    if (!channel)
    {
        client->send_reply(ERR_NOSUCHCHANNEL(client->get_nick_name(), channel_namee));
        return;
    }
    if (!channel->is_user_on_channel(client))
    {
        client->send_reply(ERR_NOTONCHANNEL(client->get_nick_name(), channel_namee));
        return;
    }
    Client* target = srv->get_client(nickName);
    if (!target)
    {
        client->send_reply(ERR_NOSUCHNICK(client->get_nick_name(), nickName));
        return;
    }
    if (!channel->is_user_on_channel(target))
    {
        client->send_reply(ERR_USERNOTINCHANNEL(client->get_nick_name(), nickName, channel_namee));
        return;
    }
    if (channel->get_channel_owner()->get_nick_name() == target->get_nick_name())
    {
        client->send_reply(ERR_NOKICKCHANNELOWNER(client->get_nick_name(), channel_namee));
        return;
    }
    if (client->is_operator())
    {
        channel->remove_user_from_channel(target);
        target->remove_channel(channel);
        string message = ":" + client->get_prefix() + " KICK " + channel_namee + " " + nickName + " :" + reason + "\n";
        channel->broadcast_message(message, client);
        target->send_message(":" + client->get_prefix() + " KICK " + channel_namee + " " + nickName + " :" + reason);
        target->send_message("You have been kicked from channel " + channel_namee + " by " + client->get_nick_name());
        string leavemessage = client->get_nick_name() + " kicked " + target->get_nick_name() + " from channel " + channel->get_channel_name();
        log(leavemessage);
    }
    else
    {
        client->send_reply(ERR_CHANOPRIVSNEEDED(client->get_nick_name(), channel_namee));
        return;
    }
    if (channel->get_channel_client_count() == 0)
    {
        string channel_name = channel->get_channel_name();
        srv->remove_channel(channel_name);
    }
}
