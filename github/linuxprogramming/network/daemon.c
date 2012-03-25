#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
int daemonize()
{
	pid_t pid = fork();
	if (-1 == pid)
		return -1;
	else if (0 != pid)
		exit(0);

	setsid();

	signal(SIGHUP, SIG_IGN);

	pid = fork();
	if (-1 == pid)
		return -1;
	else if (0 != pid)
		exit(0);

	umask(0);

	return 0;
}

int main()
{
	daemonize();
	while(1);
	return 0;
}
