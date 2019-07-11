#include <unistd.h>
#include <stdio.h>

// 实现一个和 dup2 功能相同的函数
int dup2_ex(int oldfd, int newfd)
{
	int begin_fd = oldfd;
	int cur_fd = oldfd;

	if (oldfd < 0 || newfd < 0) {
		return -1;
	}

	if (oldfd == newfd) {
		return newfd;
	}

	close(newfd);

	while (cur_fd != newfd) {
		cur_fd = dup(oldfd);
	}

	while (begin_fd != cur_fd) {
		close(begin_fd++);
	}

	return cur_fd;
}

int main(int argc, char *argv[])
{
	int fd = 20;
	int cpfd = dup2_ex(STDIN_FILENO, fd);

	printf("result = %d\n", cpfd);

	close(cpfd);

	return 0;
}

