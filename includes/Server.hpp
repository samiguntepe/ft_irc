#pragma once

#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <vector>
#include <map>
#include <fcntl.h>
#include "Client.hpp"
#include "Channel.hpp"
#include "Utils.hpp"

class Server;
typedef void (Server::*Commands)(std::vector<std::string>& , Client&);
typedef std::vector<Client>::iterator cliIt;
typedef std::vector<Channel>::iterator chanIt;

class Server
{
    private:

        int _serverFd;
        int _botFd;
        size_t _port;
        std::string _password;
        char _buffer[1024];
        std::map<std::string, Commands> _commands;
        std::vector<Channel> _channels;
        std::vector<Client> _clients;
        
        fd_set _readFds;
        fd_set _writeFds;
        fd_set _readFdsSup;
        fd_set _writeFdsSup;
        int _fdCount;
        bool flag;

        Server();
        ~Server();
        std::map<std::string, std::vector<std::string> > getParams(std::string const& str);
        static Server* singleton;
        void show_right_gui(Client &cli, Channel &cha);
        int is_nick_exist(std::string const&);
        int get_op_fd(std::string const& opName);
        int client_is_in_there(Client&, std::string const&);
        int is_channel_exist(std::string const&);
        int check_nick_depends(std::vector<std::string>&, Client&);
        void to_channel(std::vector<std::string>&, Client&);
        void to_client(std::vector<std::string>&, Client&);
        void create_socket();
        void bind_socket(size_t const&);
        void set_port(size_t const&);
        void set_password(std::string const&);
        void print_status();
        void accept_request();
        void read_event(int*);
        void write_event();
        void init_fds();
        void init_commands();
        void run();
        void pass_checker(Client&);
        void kick_client(cliIt&);
        void command_handler(std::string&, Client&);
        void modes_op(chanIt&, std::vector<std::string>& params, int* flag);
        void modes_limit(chanIt&, std::vector<std::string>& params, int* flag);
        void modes_key(chanIt&, std::vector<std::string>& params, int* flag);
        void get_after_colon(std::vector<std::string>& params);
        Channel& get_channel(std::string const&);


        void Pass(std::vector<std::string>&, Client&);
        void Nick(std::vector<std::string>&, Client&);
        void Join(std::vector<std::string>&, Client&);
        void Cap(std::vector<std::string>&, Client&);
        void User(std::vector<std::string>&, Client&);
        void Mode(std::vector<std::string>&, Client&);
        void Who(std::vector<std::string>&, Client&);
        void Quit(std::vector<std::string>&, Client&);
        void Part(std::vector<std::string>&, Client&);
        void Privmsg(std::vector<std::string>&, Client&);
        void Kick(std::vector<std::string>&, Client&);
        void Topic(std::vector<std::string>&, Client&);
        void Notice(std::vector<std::string>&, Client&);
        void Invite(std::vector<std::string>&, Client&);
        void Bot(std::vector<std::string>&, Client&);
    public:
        void manage_server(size_t const &, std::string const &);
        static Server* get_instance();
        static void signal_handler(int);
};
