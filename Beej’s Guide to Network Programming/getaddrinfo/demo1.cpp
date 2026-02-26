#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <iostream>
int main()
{
	struct addrinfo hints = {0};
	struct addrinfo *res;

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_CANONNAME;
	int d = getaddrinfo(NULL, "http", &hints, &res);
	if (d != 0)
	{
		printf("%s\n", gai_strerror(d));
	}
	std::cout << d << std::endl;
	struct addrinfo *r;
	for (r = res; r != NULL; r = r->ai_next)
	{
		std::cout << r->ai_family << std::endl;
	}
}
