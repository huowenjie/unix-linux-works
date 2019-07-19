#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <libgen.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define CMD_SIZE 64

void proc_end(int signo) 
{
	// 收到退出信号后，父进程在中断过程中检查子进程的结束状态
	pid_t child = 0;
	int status = 0;

	while ((child = waitpid(-1, &status, WNOHANG)) > 0) {
		printf("pid = %d, status---->%d\n", getpid(), status);
	}

	if (child == -1 && errno != ECHILD) {
		printf("waitpid error!\n");
		_exit(-1);
	}
}

int main(int argc, char *argv[])
{
	struct sigaction act;
	sigset_t block;
	sigset_t mask;

	pid_t zb_pid = 0;
	char cmd_buf[CMD_SIZE] = { 0 };

	// 初始化并注册信号回调
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	act.sa_handler = proc_end;

	if (sigaction(SIGCHLD, &act, NULL) < 0) {
		_exit(-1);
	}

	// 设置阻塞信号
	sigemptyset(&block);
	sigaddset(&block, SIGCHLD);
	if (sigprocmask(SIG_SETMASK, &block, NULL) == -1) {
		_exit(-1);
	}

	switch (zb_pid = fork()) {
	case -1:
		printf("create proc error!\n");
		_exit(-1);
		
	case 0:	// child
		printf("The child will be zombie! -- pid = %d, ppid = %d\n", getpid(), getppid());
		_exit(0);

	default: // parent
		printf("Parent sleep...\n");
		sleep(3);
		
		snprintf(cmd_buf, CMD_SIZE, "ps -aux | grep %s", basename(argv[0]));
		cmd_buf[CMD_SIZE - 1] = '\0';
		system(cmd_buf);

		if (kill(zb_pid, SIGKILL) < 0) {
			printf("kill signale error!\n");
			_exit(-1);
		}

		sleep(3);
		printf("Let us see child proc after send kill sig.\n");
		system(cmd_buf);

		// 父进程等待子进程信号, 如果子进程没有退出，那么阻塞当前父进程等待
		sigemptyset(&mask);
		if (sigsuspend(&mask) == -1 && errno != EINTR) {
			_exit(-1);
		}
	}

	return 0;
}

