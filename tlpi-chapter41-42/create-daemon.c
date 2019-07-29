#include "create-daemon.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>

static int counter = 0;

static void sig_handle(int sig) 
{
	counter++;
}

void create_daemon()
{
	int fd = 0;
	pid_t pid = 0;
	char buff[16] = { 0 };

	struct sigaction sa = { 0 };

	if ((pid = fork()) < 0) {
		// error
		_exit(-1);
	} else if (pid > 0) {
		// parent
		_exit(pid);
	}

	if (setsid() == -1) {
		_exit(-1);
	}

	if ((pid = fork()) < 0) {
		_exit(-1);
	} else if (pid > 0) {
		_exit(pid);
	}

	// 保持进程对文件目录的访问权限
	umask(0);

	// 设置工作目录
	chdir(".");

	sigemptyset(&sa.sa_mask);
	sa.sa_handler = sig_handle;
	sa.sa_flags = SA_RESTART;

	if (sigaction(SIGALRM, &sa, NULL) == -1) {
		_exit(-1);
	}

	// 每隔 5s 写入一次文件
	if ((fd = open("test.log", O_CREAT | O_RDWR | O_TRUNC, S_IRWXU | S_IRWXG | S_IRWXO)) < 0) {
		_exit(fd);
	}

	while (1) {
		// 注册闹钟响应信号
		alarm(5);

		sprintf(buff, "--->%d\n", counter);
		if (write(fd, &buff, strlen(buff)) < 0) {
			break;
		}

		if (counter >= 10) {
			break;
		}

		pause();
	}

	close(fd);
}
