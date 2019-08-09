#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <errno.h>

#define BUFF_SIZE 64
#define DEF_PORT  9999
#define BACKLOG   10

// 处理连接任务
void conn_handle(int fd)
{
	char   buff[BUFF_SIZE] = { 0 };
	size_t buf_size = 0;
	size_t rd_size  = 0;
	time_t tm	 	= 0;

	while ((rd_size = read(fd, buff, BUFF_SIZE)) > 0) {
		// 确保缓冲区末尾为 0
		buff[rd_size - 1] = '\0';
		
		// 退出
		if (!strcmp(buff, "server quit")) {
			// 客户端发出退出指令，向客户端发送应答响应
			strcpy(buff, "server will quit!");
			buf_size = strlen(buff);

			// 这里不论是否写入成功，都会直接退出本次任务
			write(fd, buff, buf_size);
			break;

		// 获取服务器时间
		} else if (!strcmp(buff, "time")) {
			tm = time(NULL);
			strncpy(buff, ctime(&tm), BUFF_SIZE);
			buff[BUFF_SIZE - 1] = '\0';
			buf_size = strlen(buff);

			// 回写客户端
			if (write(fd, buff, buf_size) != buf_size) {
				break;
			}
		} else {
			strcpy(buff, "Please enter quit or time!\n");
			buf_size = strlen(buff);
	
			if (write(fd, buff, buf_size) != buf_size) {
				break;
			}
		}
	}
}

// 子进程终止时， 会给父进程发送 SIGCHLD 信号
void sig_handle(int sig)
{
	pid_t child = 0;
	int status = 0;

	while ((child = waitpid(-1, &status, WNOHANG)) > 0) {
		printf("process %d will be exited!\n", child);
	}
}

int main(int argc, char *argv[])
{
	int lfd = 0;	// 监听 socket
	int cfd = 0;	// 连接 socket

	struct sockaddr_in saddr;	// ipv4 服务器地址信息
	struct sockaddr_in caddr;	// ipv4 客户端地址信息
	socklen_t clen = 0;			// 客户端地址信息长度
	pid_t cpid = 0;				// 子进程进程号

	const char *str_addr = NULL;
	unsigned int port = 0;
	char buff[BUFF_SIZE] = { 0 };

	// 捕获子进程终止的信号
	struct sigaction sa = { 0 };

	sigemptyset(&sa.sa_mask);
	sa.sa_handler = sig_handle;
	sa.sa_flags |= SA_RESTART;

	// 注册子进程结束信号
	if (sigaction(SIGCHLD, &sa, NULL) < 0) {
		_exit(-1);
	}

	if ((lfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		_exit(-1);
	}

	memset(&saddr, 0, sizeof(saddr));
	memset(&caddr, 0, sizeof(caddr));

	saddr.sin_family = AF_INET;
	saddr.sin_port   = htons(DEF_PORT);
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);

	// 绑定 socket
	if (bind(lfd, (const struct sockaddr *)&saddr, sizeof(saddr)) < 0) {
		_exit(-1);
	}

	// 监听 socket
	if (listen(lfd, BACKLOG) < 0) {
		_exit(-1);
	}

	for (;;) {
		// 等待客户端连接
		cfd = accept(lfd, (struct sockaddr *)&caddr, &clen);
		if (cfd < 0) {
			if (errno == EINTR) {
				continue;
			} else {
				_exit(-1);
			}
		}

		str_addr = inet_ntop(AF_INET, &caddr, buff, sizeof(buff));
		port = ntohs(caddr.sin_port);
		printf("connection from %s:%d\n", (str_addr ? str_addr : ""), port);

		if ((cpid = fork()) == 0) {
			// 创建子进程处理任务
			close(lfd);
			printf("new task, parent is %d, cur is %d\n", getppid(), getpid());

			// 处理任务
			conn_handle(cfd);

			printf("this task will be exited!\n");
			close(cfd);
			_exit(0);
		}

		if (close(cfd) < 0) {
			_exit(-1);
		}
	}

	close(lfd);

	return 0;
}
