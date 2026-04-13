#include <iostream>
#include <netdb.h>
#include <arpa/inet.h>
int main()
{
	struct sockaddr_in in;
	char addr[INET6_ADDRSTRLEN];
	
	int status = inet_pton(AF_INET,"127.0.0.1", &(in.sin_addr));
	std::cout << status;
	

	const char *ip = inet_ntop(AF_INET, &(in.sin_addr), addr, sizeof addr);
	std::cout << "return value " << ip << std::endl;
	std::cout << addr;

	
}
