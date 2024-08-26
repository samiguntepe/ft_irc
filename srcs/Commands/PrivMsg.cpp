#include "../../includes/Commands.hpp"

// PRIVMSG komutunu işleyen fonksiyon
void PrivMsg::privMsg(Client *client, vector<string> commandParts, Server *srv)
{
    if (commandParts.size() < 2)
    {
        client->send_reply(ERR_NEEDMOREPARAMS(client->get_nick_name(), "PRIVMSG"));
        return;
    }
    string commandString = merge_string(commandParts, " ");
    size_t targetStart = commandString.find("PRIVMSG") + 8;
    size_t messageStart = commandString.find(" :", targetStart);
    if (messageStart == string::npos)
    {
        client->send_reply(ERR_NOTEXTTOSEND(client->get_nick_name()));
        return;
    }
    string target = commandString.substr(targetStart, messageStart - targetStart);
    target = strim(target);
    string message = commandString.substr(messageStart + 2);
    if (target.at(0) == '#')
    {
        send_channel_message(client, target, message, srv);
    }
    else
    {
        send_private_message(client, target, message, srv);
    }
}

void send_channel_message(Client *client, string channelName, string message, Server *srv)
{
    string bot_name = "reepNaoBot";
    Client *bot = srv->get_client(bot_name);
    Channel *channel = srv->get_channel(channelName);
    if (channel == NULL)
    {
        client->send_reply(ERR_NOSUCHCHANNEL(client->get_nick_name(), channelName));
        return;
    }
    if (channel->get_moderated() && !client->is_operator())
    {
        client->send_reply(ERR_NOCANNOTSENDTOCHAN(client->get_nick_name(), channelName));
        return;
    }
    if (channel->get_no_external_messages() && !channel->is_user_on_channel(client))
    {
        client->send_reply(ERR_NOCANNOTSENDTOCHAN(client->get_nick_name(), channelName));
        return;
    }
    channel->broadcast_message(":" + client->get_prefix() + " PRIVMSG " + channelName + " :" + message, client);
    bot->send_message(":" + client->get_prefix() + " PRIVMSG " + channelName + " :" + message);
}

void send_private_message(Client *client, string target, string message, Server *srv)
{
    Client *target_client = srv->get_client(target);
    string bot_name = "reepNaoBot";
    Client *bot = srv->get_client(bot_name);
    if (target_client == NULL)
    {
        client->send_reply(ERR_NOSUCHNICK(client->get_nick_name(), target));
        return;
    }
    target_client->send_message(":" + client->get_prefix() + " PRIVMSG " + target + " :" + message);
    if (bot_name != target)
        bot->send_message(":" + client->get_prefix() + " PRIVMSG " + target + " :" + message);
}
