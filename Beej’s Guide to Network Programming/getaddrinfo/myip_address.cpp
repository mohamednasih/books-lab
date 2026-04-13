#include <iostream>

#include <netdb.h>
#include <arpa/inet.h>
int 	main()
{
	struct addrinfo hints = {};	
	struct addrinfo *res = NULL;
	char	ip[INET6_ADDRSTRLEN];
	struct sockaddr_in *ipv4;
	struct sockaddr_in6 *ipv6;
	void				*addr;
	std::string ip_family;

	hints.ai_family = AF_UNSPEC;
	hints.ai_flags = AI_PASSIVE;
	int getaddrinfostatus = getaddrinfo(NULL, "1337", &hints, &res);
	if (getaddrinfostatus != 0)
	{
		std::cout << gai_strerror(getaddrinfostatus) << std::endl;
		return (EXIT_FAILURE);
	}
	
	while (res)
	{
		if (res->ai_family == AF_INET)
		{
			ipv4 = reinterpret_cast<struct sockaddr_in *>(res->ai_addr);
			ip_family = "IPV4";	
			addr = &(ipv4->sin_addr);
		}
		else 
		{
			ipv6 = reinterpret_cast<struct sockaddr_in6 *>(res->ai_addr);
			ip_family = "IPV4";	
			addr = &(ipv6->sin6_addr);
		}
		inet_ntop(res->ai_family, addr, ip, sizeof ip);
		std::cout << ip_family << " " << ip << std::endl;
		res = res->ai_next;
	}
}
