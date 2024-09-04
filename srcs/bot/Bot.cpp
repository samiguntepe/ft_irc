#include "../../includes/Bot.hpp"

Bot* Bot::singleton = NULL;

Bot* Bot::get_instance()
{
    try {
        if (singleton == NULL) 
            singleton = new Bot;
        return singleton;
    } catch (std::exception & e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
}

Bot::~Bot()
{
    if (singleton != NULL)
        delete singleton;
    singleton = NULL;
    close(_fd);
}

void Bot::set_pasword(std::string const& password)
{
    _password = password;
}

void Bot::set_port(int port)
{
    _port = port;
}

void Bot::create_socket()
{
    if ((_fd = socket(AF_INET, SOCK_STREAM, 0)) <= 0) 
        throw std::runtime_error("socket() failed");
    
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    
    addr.sin_family = AF_INET;
    addr.sin_port = htons(_port);
    
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
    if (::connect(_fd, (struct sockaddr *) &addr, sizeof(addr)) < 0)
        throw std::runtime_error("connect() failed");

    std::cout << RED << "JARVIS " << BLUE << "Bot connected to server" << RESET << std::endl;
}

void Bot::run()
{
    int login = 0;

    while (1)
    {
        if (!login)
        {
            std::string msg = "CAP BOT\nPASS " + _password + "\nNICK " + RED "JARVIS_BOT\n";
            Utils::write_message(_fd, msg);
            login = 1;
        }
        char buffer[1024];
        int bytes_received = recv(_fd, buffer, sizeof(buffer), 0);
        buffer[bytes_received] = '\0';
        std::string tmp = buffer;

        if (bytes_received <= 0)
            throw std::runtime_error("recv() failed");

        std::stringstream ss(tmp);
        std::string ctr;
        ss >> ctr;

        if (ctr == "bot")
        {
            ss >> tmp; 
            ss >> ctr; 
            std::string msg = ctr;
            while (ss >> ctr)
                msg += " " + ctr;
            std::string writeCmd = "NOTICE * " + tmp + " " + msg + "\r\n";
            Utils::write_message(_fd, writeCmd);
        }
    }
}

void Bot::manage_bot(int port, std::string const& password)
{
    set_pasword(password);
    set_port(port);
    create_socket();
    run();
}