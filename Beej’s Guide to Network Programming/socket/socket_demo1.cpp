#include <iostream>
#include <netdb.h>


int	main(int ac, char **av)
{
	struct addrinfo hints {0};
	struct addrinfo *res;

	hints.ai_family = AF_UNSPEC;
	int status = getaddrinfo(av[2], av[1], &hints, &res);
	if (status != 0)
	{
		std::cout << "couldn't get addrinfo because " << gai_strerror(status) << std::endl;
		return (1);
	}
	int socketfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (socketfd == -1)
	{
		perror("socket erro");
		return (1);
	}
}
