#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
    // Soket oluşturma
    int socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socket_fd < 0)
    {
        std::cerr << "Socket creation failed." << std::endl;
        return 1;
    }

    // Soket özelliklerinin ayarlanması
    sockaddr_in socket_feature;
    std::memset(&socket_feature, 0, sizeof(socket_feature));
    socket_feature.sin_family = AF_INET;
    socket_feature.sin_port = htons(6667);
    socket_feature.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Soket bağlama
    if (bind(socket_fd, (struct sockaddr *)&socket_feature, sizeof(socket_feature)) < 0)
    {
        std::cerr << "Bind failed." << std::endl;
        close(socket_fd);
        return 1;
    }

    // Bağlantı dinleme
    if (listen(socket_fd, 5) < 0)
    {
        std::cerr << "Listen failed." << std::endl;
        close(socket_fd);
        return 1;
    }

    std::cout << "Waiting for connections..." << std::endl;

    sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buff[1024];

    // Bağlantı kabul etme
    int client_fd = accept(socket_fd, (struct sockaddr *)&client_addr, &client_len);
    if (client_fd < 0)
    {
        std::cerr << "Accept failed." << std::endl;
        close(socket_fd);
        return 1;
    }

    std::cout << "Client connected" << std::endl;

    // Veri alma
    ssize_t bytes_received = recv(client_fd, buff, sizeof(buff) - 1, 0);
    if (bytes_received > 0)
    {
        buff[bytes_received] = '\0'; // String null terminator
        std::cout << "Received message: " << buff << std::endl;
    }
    else
    {
        std::cerr << "Receive failed." << std::endl;
    }

    // Soketleri kapatma
    close(client_fd);
    close(socket_fd);

    return 0;
}
