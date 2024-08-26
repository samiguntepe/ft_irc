#include "../../includes/Commands.hpp"

// PASS komutunu işleyen fonksiyon
void Pass::pass(Client *client, const vector<string> commandParts, Server *srv)
{
    if (client->is_registered() || client->get_user_auth() || client->get_is_pass())
    {
        client->send_reply(ERR_ALREADYREGISTERED(client->get_nick_name()));
        return;
    }
    if (commandParts.size() < 2)
    {
        client->send_reply(ERR_NEEDMOREPARAMS(client->get_nick_name(), "PASS"));
        return;
    }
    string passw = commandParts.at(1);
    if (!srv->verify_server_password(passw))
    {
        client->send_message("Access denied!");
        return;
    }
    client->set_pass(true);
    client->send_message("Password accepted!");
}
