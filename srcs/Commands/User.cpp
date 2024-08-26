#include "../../includes/Commands.hpp"

// USER komutunu işleyen fonksiyon
void User::user(Client *client, vector<string> commandParts, Server *srv)
{
    if (!client || !srv)
    {
        write(2, "Error: client or srv is null\n", 29);
        return;
    }
    if (client->is_registered())
    {
        client->send_reply(ERR_ALREADYREGISTERED(client->get_nick_name()));
        return;
    }
    if (commandParts.size() < 3)
    {
        client->send_reply(ERR_NEEDMOREPARAMS(client->get_nick_name(), "USER"));
        return;
    }
    if (!client->get_valid_name(commandParts.at(1)) || !client->get_valid_name(commandParts.at(2)))
    {
        client->send_reply(ERR_ERRONEUSNICKNAME(client->get_nick_name(), commandParts.at(1)));
        return;
    }
    std::string userName = commandParts.at(1);
    std::string realName = commandParts.at(2);
    const std::map<int, Client *> &clients = srv->get_all_clients();
    for (std::map<int, Client *>::const_iterator it = clients.begin(); it != clients.end(); ++it)
    {
        const Client *regUser = it->second;
        if (!regUser)
        {
            continue;
        }
        if (regUser != client && regUser->get_user_name() == userName)
        {
            client->send_message("This username is already in use");
            return;
        }
    }
    client->set_user_name(userName);
    client->set_real_name(realName);
    client->set_status(CLIENT_CONNECTED);
    client->set_user_auth(true);
    if (client->get_user_auth() == true)
    {
        Bot *bot = srv->get_bot();
        if (bot)
            bot->welcome_message(client->get_nick_name());
    }
}
