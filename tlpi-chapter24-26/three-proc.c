#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>

#define CMD_SIZE 64

int main(int argc, char *argv[])
{
	pid_t son = 0;
	pid_t grandson = 0;
	char  cmd_buf[CMD_SIZE] = { 0 };

	snprintf(cmd_buf, CMD_SIZE, "ps -aux | grep %s", basename(argv[0]));
	cmd_buf[CMD_SIZE - 1] = '\0';

	if ((son = fork()) < 0) {
		_exit(-1);
	} else if (son == 0) {
		// father
		sleep(2);
		printf("father pid = %d, ppid = %d\n", getpid(), getppid());
		
		if ((grandson = fork()) < 0) {
			_exit(-1);
		} else if (grandson == 0) {
			sleep(5);
			printf("son pid = %d, ppid = %d\n", getpid(), getppid());
		} else {
			_exit(0);
		}
	} else {
		// grandfather
		sleep(3);
		printf("gradfather pid = %d, son id = %d\n", getpid(), son);
		system(cmd_buf);
		wait(NULL);
		sleep(6);
	}

	return 0;
}

