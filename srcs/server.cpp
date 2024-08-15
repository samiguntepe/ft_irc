#include "../includes/Server.hpp"

Server* Server::ins = NULL;

Server::Server(int server_socket_family, int server_socket_protocol, string server_name)
	: server_fd(-1),
	server_socket_family(server_socket_family),
	server_socket_protocol(server_socket_protocol),
	port_number(0),
	server_name(server_name),
	password(""),
	_bot(NULL)
{
	signal(SIGINT, signal_handler);
	Server::set_instance(this);
	memset(&server_address, 0, sizeof(server_address));
	FD_ZERO(&read_set);

}

Server::~Server()
{
	Server::ins = NULL;
	for (std::map<int, Client*>::iterator it = clients.begin(); it != clients.end(); ++it) {
		if (it->second != NULL)
			delete it->second;
	}
	if (!clients.empty())
		clients.clear();
	for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it) {
		if (it->second != NULL)
			delete it->second;
	}
	if (!_channels.empty())
		clients.clear();
	if (server_fd != -1)
		close(server_fd);
	FD_ZERO(&read_set);
	if (_bot != NULL){
		delete _bot;
		_bot = NULL;
	}
}

void Server::socket_start()
{
	server_fd = socket(server_socket_family, server_socket_protocol, 0);
	if (server_fd == -1)
		throw RuntimeError("Socket creation failed.");
	if (fcntl(server_fd, F_SETFL, O_NONBLOCK) == -1)
	{
		close(server_fd);
		RuntimeError("Failed to set socket to non-blocking");
	}
	int reuse = 1;
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) == -1)
	{
		close(server_fd);
		RuntimeError("Failed to set socket options");
	}
}

void Server::socket_init()
{
	switch (server_socket_family)
	{
		case AF_INET:
			server_address.sin_addr.s_addr = INADDR_ANY;
			server_address.sin_family = server_socket_family;
			server_address.sin_port = htons(port_number);
			break;
			
		default:
			close(server_fd);
			RuntimeError("Not supported domain");
	}
}

void Server::socket_bind()
{
	if (::bind(server_fd, reinterpret_cast<struct sockaddr*>(&server_address), sizeof(server_address)) == -1)
	{
		close(server_fd);
		RuntimeError("Failed to bind socket");
	}
}

void Server::socket_listen()
{
	if (listen(server_fd, BACKLOG_SIZE) == -1 )
	{
		close(server_fd);
		RuntimeError("Failed to listen socket");
	}
	FD_SET(server_fd, &read_set);
}


int Server::socket_accept()
{	
	struct sockaddr_storage client_address;
	socklen_t client_address_length = sizeof(client_address);
	int client_socket_fd = accept(server_fd, (struct sockaddr *)&client_address, &client_address_length);
	if (client_socket_fd < 0)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
		{
		}
		else
		{
			RuntimeError("Failed to accept socket");
		}
	}
	if (fcntl(client_socket_fd, F_SETFL, O_NONBLOCK) == -1)
	{
		close(client_socket_fd);
		RuntimeError("Failed to set socket to non-blocking");
	}
	int reuse = 1;
	if (setsockopt(client_socket_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) == -1)
	{
		close(client_socket_fd);
		RuntimeError("Failed to set socket options");
	}
	char hostname[NI_MAXHOST];
	if (inet_ntop(AF_INET, &(((struct sockaddr_in *)&client_address)->sin_addr), hostname, sizeof(hostname)) == NULL)
	{
		RuntimeError("Failed to getaddrinfo socket");
	}
	Client* client = NULL;
	client = new Client(client_socket_fd, ntohs(((struct sockaddr_in*)&client_address)->sin_port), hostname, server_name);
	clients.insert(std::make_pair(client_socket_fd, client));
	std::ostringstream message_stream;
	message_stream << "\tNew Client: has connected.";
	client->sendMessage("Connected to Server");
	client->sendMessage("Enter the server password using /PASS");
	log(message_stream.str());
	return client_socket_fd;
}

void Server::start()
{
	socket_start();
	socket_init();
	socket_bind();
	socket_listen();
	signal(SIGINT, signal_handler);
	try
	{
		_bot = new Bot("localhost", port_number, password);
	}
	catch (const std::exception &e)
	{
		delete _bot;
		_bot = NULL;
		write(STDOUT_FILENO, e.what(), strlen(e.what()));
	}
	while (true)
	{
		int max_fd = _bot->get_socket();
		int n = 0;
		FD_ZERO(&read_set);
		FD_SET(server_fd, &read_set);
		FD_SET(_bot->get_socket(), &read_set);
		for (map<int, Client*>::iterator it = clients.begin(); it != clients.end(); it++)
		{
			FD_SET((*it).second->get_client_socket_fd(), &read_set);
			max_fd = std::max(max_fd, (*it).second->get_client_socket_fd());
		}
		n = select(max_fd + 1, &read_set, NULL, NULL, NULL);
		if(n)
		{
			if (FD_ISSET(server_fd, &read_set))
				socket_accept();
			for (map<int, Client*>::iterator it = clients.begin(); it != clients.end(); it++)
			{
				if (FD_ISSET((*it).second->get_client_socket_fd(), &read_set))
				{
					handle_client((*it).second->get_client_socket_fd());
					break;
				}
			}
			if (FD_ISSET(_bot->get_socket(), &read_set))
				_bot->listen(this);
		}
	}
}

void Server::arg_control(char **argv)
{
	port_number = 0;

	if (argv[1] != NULL) {
		port_number = std::atoi(argv[1]);
	} else {
		throw RuntimeError("Usage: ./server <port_number> <password>");
	}
	if (port_number <= 0)
	{
		throw RuntimeError("Invalid port number.");
	}
	if (port_number < 1024 || port_number > 65535)
	{
		throw RuntimeError("Invalid port."); 
	}
	std::cout << port_number << std::endl;
	this->password = argv[2];

}
