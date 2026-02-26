#include <iostream>
#include <sys/socket.h>

#include <netdb.h>
#include <arpa/inet.h>

int main(int ac, char **av)
{
	struct addrinfo hints = {0};
	struct addrinfo *res;

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	int status = getaddrinfo(av[2], av[1], &hints, &res);
	if (status != 0)
	{
		std::cout << gai_strerror(status);
	}
	char ipaddres[INET6_ADDRSTRLEN];
	struct sockaddr_in *ipv4;	
	struct sockaddr_in6 *ipv6;	
	void *addr;
	while (res)
	{
		std::string ipFamilly;
				
		if (res->ai_family == AF_INET)
		{
			ipFamilly = "IPV4";
			ipv4 = reinterpret_cast<struct sockaddr_in *>(res->ai_addr);	
			addr = &(ipv4->sin_addr);
		}
		else 
		{
			ipFamilly = "IPV6";
			ipv6 = reinterpret_cast<struct sockaddr_in6 *>(res->ai_addr); 
			addr=&(ipv6->sin6_addr);
		}
		inet_ntop(res->ai_family, addr, ipaddres, INET6_ADDRSTRLEN);
		std::cout << ipFamilly << " " <<  ipaddres << std::endl;
		res = res->ai_next;
	}


}

/*

	not that good
*/
