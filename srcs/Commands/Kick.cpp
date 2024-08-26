#include "../../includes/Commands.hpp"

void Kick::kick(Client* client, vector<string> commandParts, Server* srv)
{
    if (commandParts.size() < 3)
    {
        client->send_reply(ERR_NEEDMOREPARAMS(client->get_nick_name(), "KICK"));
        return;
    }
    string channelNamee = commandParts.at(1);
    string nickName = commandParts.at(2);
    string reason = (commandParts.size() > 3) ? commandParts.at(3) : "No reason specified";
    if (channelNamee.at(0) != '#')
    {
        client->send_reply(ERR_NOSUCHCHANNEL(client->get_nick_name(), channelNamee));
        return;
    }
    Channel* channel = srv->get_channel(channelNamee);
    if (!channel)
    {
        client->send_reply(ERR_NOSUCHCHANNEL(client->get_nick_name(), channelNamee));
        return;
    }
    if (!channel->is_user_on_channel(client))
    {
        client->send_reply(ERR_NOTONCHANNEL(client->get_nick_name(), channelNamee));
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
        client->send_reply(ERR_USERNOTINCHANNEL(client->get_nick_name(), nickName, channelNamee));
        return;
    }
    if (channel->get_channel_owner()->get_nick_name() == target->get_nick_name())
    {
        client->send_reply(ERR_NOKICKCHANNELOWNER(client->get_nick_name(), channelNamee));
        return;
    }
    if (client->is_operator())
    {
        channel->remove_user_from_channel(target);
        target->remove_channel(channel);
        string message = ":" + client->get_prefix() + " KICK " + channelNamee + " " + nickName + " :" + reason + "\n";
        channel->broadcast_message(message, client);
        target->send_message(":" + client->get_prefix() + " KICK " + channelNamee + " " + nickName + " :" + reason);
        target->send_message("You have been kicked from channel " + channelNamee + " by " + client->get_nick_name());
    }
    else
    {
        client->send_reply(ERR_CHANOPRIVSNEEDED(client->get_nick_name(), channelNamee));
        return;
    }
    if (channel->get_channel_client_count() == 0)
    {
        string channelName = channel->get_channel_name();
        srv->remove_channel(channelName);
    }
}
