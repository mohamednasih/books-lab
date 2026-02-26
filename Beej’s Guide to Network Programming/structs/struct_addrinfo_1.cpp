#include <iostream>
#include <netinet/in.h>
#include <netdb.h>
const char* protocol_name(int protocol) {
	switch (protocol) {
		case 0: return "IP"; // 0 is IP for both IPv4 and IPv6
		case IPPROTO_TCP: return "TCP";
		case IPPROTO_UDP: return "UDP";
		case IPPROTO_ICMP: return "ICMP";
		case IPPROTO_ICMPV6: return "ICMPV6";
		default: return "UNKNOWN";
	}
}

const char* family_name(int family) {
	switch (family) {
		case AF_UNSPEC: return "AF_UNSPEC";
		case AF_INET: return "AF_INET";
		case AF_INET6: return "AF_INET6";
		case AF_UNIX: return "AF_UNIX";
		default: return "UNKNOWN";
	}
}

const char* socktype_name(int socktype) {
	switch (socktype) {
		case SOCK_STREAM: return "SOCK_STREAM";
		case SOCK_DGRAM: return "SOCK_DGRAM";
		case SOCK_RAW: return "SOCK_RAW";
		default: return "UNKNOWN";
	}
}

// void print_sockaddr(void *p, int ai_family)
// {
// 	if (ai_family != AF_INET)
// 	{
// 		struct sockaddr_in *addr = p;
// 		std::cout << "port" << p->port;
// 		std::cout << ""
// 	}
// 	else
// 	{

// 	}
// }
int main(int ac, char **av)
{
	struct addrinfo hints = {0};
	struct addrinfo *res = NULL;
	hints.ai_family = AF_INET6;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;	
	int status = getaddrinfo(NULL, av[2], &hints, &res);
	if (status != 0)
	{
		std::cout << status << " " << gai_strerror(status) << std::endl;
	}
	if (res == NULL)
	{
		std::cout << "empty resutls \n" ;
		return 0;
	}
	for (int i = 0; res != NULL ; res = res->ai_next, i++)
	{
		std::cout << "Result #" << i << std::endl;
		std::cout << "  ai_flags:    " << res->ai_flags << std::endl;
		std::cout << "  ai_family:   " << res->ai_family << " (" << family_name(res->ai_family) << ")" << std::endl;
		std::cout << "  ai_socktype: " << res->ai_socktype << " (" << socktype_name(res->ai_socktype) << ")" << std::endl;
		std::cout << "  ai_protocol: " << res->ai_protocol << " (" << protocol_name(res->ai_protocol) << ")" << std::endl;
		std::cout << "  ai_addrlen:  " << res->ai_addrlen << std::endl;
		std::cout << "  ai_canonname: ";
		if (res->ai_canonname)
			std::cout << res->ai_canonname << std::endl;
		else
			std::cout << "(null)" << std::endl;
		std::cout << "  ai_addr:     " << res->ai_addr << std::endl;
		std::cout << "  ai_next:     " << res->ai_next << std::endl;
		std::cout << "--------------------------" << std::endl;

		// Clear the hints struct (for demonstration, though not needed in this loop)
		hints = {0};
	}	
}
