#include "../../includes/Server.hpp"

void Server::Bot(std::vector<std::string>& params, Client& cli)
{
    pass_checker(cli);
    if (cli._nick == "") {
        Utils::write_message(cli._cliFd, "Set your nickname first\r\n");
        return ;
    }
    if (params[0] == "") {
        Utils::write_message(cli._cliFd, ERR_NEEDMOREPARAMS(cli._nick, "BOT"));
        return ;
    }
    if (_botFd == 0) {
        Utils::write_message(cli._cliFd, "Bot is not connected\r\n");
        return ;
    }
    std::string msg = "";
    for (size_t i = 0; i < params.size(); ++i) {
        msg += params[i] + " ";
    }
    Utils::write_message(_botFd, "bot " + cli._nick + " " + msg + "\n");
}
