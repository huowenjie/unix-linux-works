#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
	pid_t id = 0;
	char buff[16] = { 0 };

	if ((id = fork()) > 0) { // parent
		printf("Parent pid = %d, Child id = %d\n", getpid(), id);
		
		sleep(20);
		sprintf(buff, "kill -9 %d", id);
		system(buff);
	} else if (id == 0) {
		printf("Child pid = %d, Parent id = %d\n", getpid(), getppid());
		if ((id = setsid()) < 0) {
			printf("setup session error!\n");
			exit(-1);
		}
		
		printf("create session success!\n");
		while (read(STDIN_FILENO, buff, 16) > 0) {
			printf("final!\n");
		}
	} else {
		printf("create process error!\n");
		exit(-1);
	}

	return 0;
}

