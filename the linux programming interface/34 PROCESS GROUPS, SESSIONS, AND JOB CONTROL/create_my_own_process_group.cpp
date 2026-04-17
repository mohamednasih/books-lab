#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
void sigint(int signum)
{
	std::cerr << "I " << getpid() << "received signal " << signum << std::endl;
	exit(1);
} 

int main()
{
	int 	status;
	pid_t	current_prg = getpgid(0);
	pid_t	pid = getpid();
	signal(SIGINT, sigint);
	std::cout << "I am " << pid << std::endl;
	std::cout << "my current process group is " << current_prg << std::endl;
	std::cout << "I am trying to change my process group " <<  std::endl;
	status = setpgid(0, 0);
	std::cout << "my current process group is " << getpgid(0)<< std::endl;
	std::cerr << "try to kill me using ctrl c" << std::endl;
	while (1)
	;
	return (0);
}
