#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	pid_t pid;

	if ((pid = fork()) > 0) {
		printf("Create Child success! pid = %d\n", pid);
		sleep(1);
	} else if(pid == 0) {
		printf("Process child! pid = %d, ppid = %d\n", getpid(), getppid());
	} else {
		printf("Create Process error!\n");
	}

	return 0;
}

