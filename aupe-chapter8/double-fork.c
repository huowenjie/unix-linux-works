#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	pid_t pid = 0;

	if ((pid = fork()) < 0) {
		printf("fork error! pid = %d\n", getpid());
	} else if (pid == 0) {
		/* 第一个子进程 */
		if ((pid = fork()) < 0) {
			printf("fork error! 2 pid = %d\n", getpid());
		} else if (pid == 0) {
			/* 第二个子进程 */
			sleep(2);
			printf("second child pid = %d, ppid = %d\n", getpid(), getppid());
			exit(0);
		} else {
			/* 第一个子进程 */
			exit(0);
		}
	}

	if (waitpid(pid, NULL, 0) != pid) {
		printf("wait pid error! this pid = %d\n", pid);
	}

	exit(0);
		
	return 0;
}

