#include <iostream>
#include <netdb.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
int main()
{
	struct sockaddr_in ipv4;
	
	ipv4.sin_family = AF_INET;
	ipv4.sin_port = 1;
	ipv4.sin_addr.s_addr = inet_addr("0.0.0.0");
	memset(ipv4.sin_zero, 0, sizeof(ipv4.sin_zero));
	int socketfd = socket(PF_INET, SOCK_STREAM, 0);
	if (socketfd == -1)
	{
		perror("socket");
		return (EXIT_FAILURE);
	}
	int bindstatus = bind(socketfd, reinterpret_cast<struct sockaddr *>(&ipv4), sizeof(ipv4));
	if (bindstatus == -1)
	{
		perror("bind");
		return (EXIT_FAILURE); 
	}
	listen(socketfd, 10);
	pause();
	
}
