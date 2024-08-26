#include "../../includes/Commands.hpp"

static void sendNoticeChannelMessage(Client *client, string channelName, string message, Server *srv)
{
    Channel *channel = srv->get_channel(channelName);
    if (channel == NULL)
    {
        client->send_reply(ERR_NOSUCHCHANNEL(client->get_nick_name(), channelName));
        return;
    }
    channel->broadcast_message(":" + client->get_prefix() + " NOTICE " + channelName + " :" + message);
}

static void sendNoticeMessage(Client *client, string target, string message, Server *srv)
{
    Client *target_client = srv->get_client(target);
    if (target_client == NULL)
    {
        client->send_reply(ERR_NOSUCHNICK(client->get_nick_name(), target));
        return;
    }
    target_client->send_message(":" + client->get_prefix() + " NOTICE " + target + " :" + message);
}

void Notice::notice(Client *client, vector<string> commandParts, Server *srv)
{
    if (commandParts.size() < 3)
    {
        client->send_message(":" + client->get_host_name() + " 461 " + client->get_nick_name() + " NOTICE : Not enough parameters");
        return;
    }
    string commandString = merge_string(commandParts, " ");
    size_t targetStart = commandString.find("NOTICE") + 7;
    size_t messageStart = commandString.find(" :", targetStart);
    if (messageStart == string::npos)
    {
        client->send_message(":" + client->get_host_name() + " 412 " + client->get_nick_name() + "  NOTICE :No text to send");
        return;
    }
    string target = commandString.substr(targetStart, messageStart - targetStart);
    string message = commandString.substr(messageStart + 2);
    if (target.empty())
    {
        client->send_message(":" + client->get_host_name() + " 401 " + client->get_nick_name() + " NOTICE :No such nick/channel");
        return;
    }
    if (message.empty())
    {
        client->send_message(":" + client->get_host_name() + " 412 " + client->get_nick_name() + " NOTICE :No text to send");
        return;
    }
    target = strim(target);
    if (target.at(0) == '#')
    {
        sendNoticeChannelMessage(client, target, message, srv);
    }
    else
    {
        sendNoticeMessage(client, target, message, srv);
    }
}
