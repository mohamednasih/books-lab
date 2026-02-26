#include <iostream>

#include <netdb.h>
#include <unistd.h>
int main(int ac, char **av)
{
		struct addrinfo hints = {0};
		struct addrinfo *res;
		

		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		int status = getaddrinfo(av[2], av[1], &hints, &res);
		if (status != 0)
		{
			std::cerr << gai_strerror(status);
			return (EXIT_FAILURE);
		}
		int socketfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if (socketfd == -1)
		{
			perror("socket");
			return (EXIT_FAILURE);
		}
		int connectstatus = connect(socketfd, reinterpret_cast<struct sockaddr *>(res->ai_addr), res->ai_addrlen);
		if (connectstatus == -1)
		{
			perror("connect");
		}
		pause();
}
