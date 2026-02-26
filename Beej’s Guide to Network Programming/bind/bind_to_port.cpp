
#include <netdb.h>
#include <iostream>
#include <unistd.h>
int main(int ac, char **av)
{

	struct addrinfo hints = {0};
	struct addrinfo *res = NULL;
	hints.ai_family = AF_UNSPEC;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_socktype = SOCK_STREAM;
	int status = getaddrinfo(av[2], av[1], &hints, &res);	
	if (status != 0)
	{
		std::cout <<  "error" << gai_strerror(status) << std::endl;
		return (EXIT_FAILURE);
	}
	int socketfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (socketfd == -1)
	{
		perror("socket");
		return (EXIT_FAILURE);
	}
	status = bind(socketfd, res->ai_addr, res->ai_addrlen);
	if (status == -1)
	{
		perror("bind");
	}
	listen(socketfd, 10);
	while (1)
	{
	}
}
