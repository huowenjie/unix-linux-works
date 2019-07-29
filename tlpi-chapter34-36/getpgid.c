#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	pid_t s;

	printf("pid = %d\n", getpid());
	printf("pgid = %d\n", getpgrp());

	s = setsid();

	printf("setsid ret = %d\n", s);
	
	return 0;
}

