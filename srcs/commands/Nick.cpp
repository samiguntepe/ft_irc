#include "../../includes/Commands.hpp"

// NICK komutunu işleyen fonksiyon
void Nick::nick(Client *client, vector<string> commandParts, Server *srv)
{
    if (commandParts.size() < 2)
    {
        client->send_reply(ERR_NONICKNAMEGIVEN(client->get_nick_name()));
        return;
    }
    string nickName = commandParts.at(1);
    if (!client->get_valid_name(nickName))
    {
        client->send_reply(ERR_ERRONEUSNICKNAME(client->get_nick_name(), nickName));
        return;
    }
    if (client->get_nick_name() == nickName)
    {
        client->send_reply(ERR_nick_nameINUSE(client->get_nick_name(), nickName));
        return;
    }
    const std::map<int, Client *> &clients = srv->get_all_clients();
    for (std::map<int, Client *>::const_iterator it = clients.begin(); it != clients.end(); ++it)
    {
        const Client *regUser = it->second;
        if (!regUser)
        {
            continue;
        }
        if (regUser != client && regUser->get_nick_name() == nickName)
        {
            client->send_reply(ERR_nick_nameINUSE(client->get_nick_name(), nickName));
            return;
        }
    }
    client->send_message(":" + client->get_nick_name() + " NICK " + nickName);
    client->set_nick_name(nickName);
}
