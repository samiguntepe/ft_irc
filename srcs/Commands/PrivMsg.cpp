#include "../../includes/Server.hpp"

void Server::to_channel(std::vector<std::string>& params, Client& cli)
{
    if (is_channel_exist(params[0]) == 0) {
        Utils::write_message(cli._cliFd, ERR_NOSUCHCHANNEL(params[0], params[1]));
        return ;
    }
    if (client_is_in_there(cli, params[0]) == 1)
    {
        if (params[1][0] == ':') {
            get_after_colon(params);
        }
        for (cliIt it = _clients.begin(); it != _clients.end(); ++it) {
            if (it->_nick != cli._nick && client_is_in_there(*it, params[0]) == 1) {
                it->_messageBox.push_back(RPL_PRIVMSG(cli._nick, params[0], params[1]));
                FD_SET(it->_cliFd, &_writeFds);
            }
        }
    }
}

void Server::to_client(std::vector<std::string>& params, Client& cli)
{
    int flag = 0;

    for (cliIt it = _clients.begin(); it != _clients.end(); ++it) {
        if (params[0] == it->_nick) {
            if (params[1][0] == ':') {
               get_after_colon(params);
            }
            it->_messageBox.push_back(RPL_PRIVMSG(cli._nick, params[0], params[1]));
            FD_SET(it->_cliFd, &_writeFds);
            flag = 1;
            return ;
        }
    }
    if (flag == 0)
        Utils::write_message(cli._cliFd, ERR_NOSUCHNICK);
}

void Server::Privmsg(std::vector<std::string>& params, Client& cli)
{
    pass_checker(cli);
    if (params.size() < 2)
    {
        Utils::write_message(cli._cliFd, ERR_NEEDMOREPARAMS(cli._nick, params[0]));
        return ;
    }
    else if (params[0][0] == '#')
        to_channel(params, cli);
    else
        to_client(params, cli);
}
