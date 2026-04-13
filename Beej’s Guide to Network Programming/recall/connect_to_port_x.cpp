#include <iostream>
#include <arpa/inet.h>
#include <netdb.h>
int main(int ac, char **av)
{
	struct sockaddr_in ip;
	int					port;
	int 				status;
	int					socketfd;	

	if (ac != 3)
	{
		std::cerr << "invalid usage " << std::endl;
		return (EXIT_FAILURE);
	}
	port = atoi(av[2]);		
	ip.sin_port = htons(port);
	ip.sin_family  = AF_INET;
	status = inet_pton(AF_INET, av[1], &(ip.sin_addr.s_addr));
	if (status != 1)
	{
		if (status == 0)
		{
			std::cout << av[1] << " is not a valid ip address" << std::endl;
			return (EXIT_FAILURE);
		}
		perror("ip convert");
		return (EXIT_FAILURE);
	}
	socketfd = socket(ip.sin_family, SOCK_STREAM, getprotobyname("tcp")->p_proto);
	if (socketfd == -1)
	{
		perror("socket");
		return (EXIT_FAILURE);
	}
	status = connect(socketfd, (const struct sockaddr *)&ip, sizeof ip);
	if (status == -1)
	{
		perror("connect");
		return (EXIT_FAILURE);
	}
	
}
