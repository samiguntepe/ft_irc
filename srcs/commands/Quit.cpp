#include "../../includes/Commands.hpp"

void Quit::quit(Client *client, vector<string> commandParts, Server *srv)
{
    string reason = commandParts.empty() ? "Leaving from Server" : commandParts.at(0);
    if (reason.at(0) == ':')
        reason.substr(1);
    client->send_message(RPL_QUIT(client->get_prefix(), reason));
    srv->remove_client_all_channels(client);
    srv->clientDisconnect(client->get_client_socket_fd());
}
