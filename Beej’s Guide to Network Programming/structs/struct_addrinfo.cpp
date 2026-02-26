#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <stdio.h>
#include <arpa/inet.h>
#include <netinet/in.h>

void print_sockaddr_in(const struct sockaddr_in *addr) {
    char ip[INET_ADDRSTRLEN]; // space for IPv4 string

    // Convert IP from binary to string
    if (inet_ntop(AF_INET, &(addr->sin_addr), ip, sizeof(ip)) == NULL) {
        perror("inet_ntop");
        return;
    }

    // Print IP and port
    printf("IP: %s, Port: %u\n", ip, ntohs(addr->sin_port));
}

int main() {
    const char* hostname = "example.com";
    const char* port = "80";

    struct addrinfo hints;
    struct addrinfo *res, *p;

    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;        //AF_INET AF_INET6 AF_UNSPEC
    hints.ai_socktype = SOCK_STREAM;    // SOCK_STREAM SOCK_DGRAM

    // Get the addrinfo linked list
    int status = getaddrinfo(hostname, port, &hints, &res);
    if (status != 0) {
        std::cerr << "getaddrinfo error: " << gai_strerror(status) << "\n";
        return 1;
    }

    int sockfd;
    for (p = res; p != NULL; p = p->ai_next) {
		std::cout << "protocol " << p->ai_protocol << std::endl;
        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sockfd == -1) continue;

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            continue;
        }
		printf("socket(%d, %d, %d)\n", p->ai_family, p->ai_socktype, p->ai_protocol);
		printf("connect(%d, %p, %d)\n", sockfd, p->ai_addr ,p->ai_addrlen);
		print_sockaddr_in((const struct  sockaddr_in *)p->ai_addr);
        break; // connected successfully
    }

    if (p == NULL) {
        std::cerr << "Failed to connect\n";
        freeaddrinfo(res);
        return 2;
    }

    freeaddrinfo(res);
    return 0;
}
