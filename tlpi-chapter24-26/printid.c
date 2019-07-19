#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	printf("2.The pid = %d, ppid = %d\n", getpid(), getppid());
	_exit(-1);
	return 0;
}

