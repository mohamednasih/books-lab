#include <iostream>
#include <netdb.h>
#include <unistd.h>


int main(int ac, char **av)
{
	struct addrinfo hints = {0};
	struct addrinfo *res = NULL;

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	if (av[2] == NULL)
	{
		hints.ai_flags = AI_PASSIVE;
	}
	int getaddinfostatus = getaddrinfo(av[2], av[1], &hints , &res);
	if (getaddinfostatus != 0)
	{
		std::cout << gai_strerror(getaddinfostatus);
	 	return (EXIT_FAILURE);
	}
	int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (sockfd == -1)
	{
		perror("socket");
		return (EXIT_FAILURE);	
	}
	int bindstatus = bind(sockfd, reinterpret_cast<struct sockaddr *>(res->ai_addr), res->ai_addrlen);
	int listenstatus = listen(sockfd, 200);
	if (listenstatus == -1)
	{
		perror("listen");
		return (EXIT_FAILURE);
	}
	while (1)
		;
}
