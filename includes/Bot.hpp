#pragma once

#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Utils.hpp"

class Bot
{
    private:

        int _fd;
        int _port;
        std::string _password;
        static Bot* singleton;
        

        Bot(): _fd(0), _port(0), _password("") {};
        ~Bot();
        void set_port(int);
        void set_pasword(std::string const&);
        void create_socket();
        void run();
        void execute(std::string const&);
    public:
        static Bot *get_instance();
        void manage_bot(int, std::string const&);
};
