#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	if (fork() != 0) {
		_exit(0);
	}

	if (setsid() == -1) {
		_exit(0);
	}

	printf("PID = %d, PGID = %d, SID = %d\n", getpid(), getpgrp(), getsid(0));

	printf("ctermid = %s\n", ctermid(NULL));

	return 0;
}

