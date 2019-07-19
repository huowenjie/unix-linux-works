#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include "../error-handle.h"

int main(int argc, char *argv[])
{
	int i = 0;
	int sig_count = 100;
	int pid = 0;
	char buff[128] = { 0 };

	printf("enter your target pid:\n");
	fgets(buff, 128, stdin);

	pid = atoi(buff);
	printf("target pid = %d\n", pid);

	for (; i < sig_count; i++) {
		// 发送空信号
		if (kill(pid, SIGINT) < 0) {
			err_exit("kill sig error!\n");
		}

		sleep(1);
	}

	return 0;
}

