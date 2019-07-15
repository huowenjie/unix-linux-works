#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * opt -- 0 父进程等待子进程终止状态
 * opt -- 1 生成一个将死进程
 * opt -- 2 生成孤儿进程
 */

void parent_handle(int opt)
{
	switch (opt) {
		case 1:
			sleep(10);
			break;

		case 2:
			break;

		default:
			wait(NULL);
	}
}

int main(int argc, char *argv[])
{
	pid_t pid;
	int   opt = 0;

	if (argc >= 2 && argv && argv[1]) {
		opt = atoi(argv[1]);
		if (opt < 0 || opt > 2) {
			opt = 0;
		}
	}

	if ((pid = fork()) > 0) {
		printf("Create Child success! pid = %d\n", pid);
		parent_handle(opt);
	} else if(pid == 0) {
		printf("Process child! pid = %d, ppid = %d\n", getpid(), getppid());
		sleep(5);
	} else {
		printf("Create Process error!\n");
	}

	return 0;
}

