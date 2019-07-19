#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	pid_t pid = 0;
	int status = 0;

	printf("1.cur pid = %d\n", getpid());

	if ((pid = fork()) > 0) {
		printf("parent will end! child id = %d\n", pid);
		wait(&status);
		printf("final child exit status = %d\n", status);
	} else if (pid == 0) {
		printf("child start: id = %d\n", getpid());
		execl("test-printid", "./test-printid.c", NULL);
	} else {
		printf("create child error!\n");
	}

	return 0;
}

