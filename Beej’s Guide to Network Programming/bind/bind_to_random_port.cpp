#include <iostream>


#include <unistd.h>
#include <netdb.h>
int main()
{
	struct sockaddr_in in;

	in.sin_family = AF_INET;
	in.sin_port = 0;
	in.sin_addr.s_addr = INADDR_ANY;

	int fd = socket(AF_INET, SOCK_STREAM, getprotobyname("tcp")->p_proto);
	if (fd == -1)
	{
		perror("socket");
		return (EXIT_FAILURE);
	}
	int bindstatus = bind(fd, reinterpret_cast<const sockaddr *>(&in), sizeof in);
	if (bindstatus == -1)
	{
		perror("bind");
		return (EXIT_FAILURE);
	}
	listen(fd, 10);
	struct sockaddr_in in2;
	socklen_t len = sizeof in2;
	int getsocknamestatus = getsockname(fd, (struct sockaddr *)&in2 , &len);
	if (getsocknamestatus == -1)
	{
		perror("getsockname");
		return (EXIT_FAILURE);
	}
	std::cout << "port used is " << ntohs(in2.sin_port) << std::endl;
	pause();
}
