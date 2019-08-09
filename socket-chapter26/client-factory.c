#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TARGET_PORT 9999
#define TARGET_ADDR "127.0.0.1"
#define PROC_PRINT(str) printf("%s -- PID = %d\n", (str), getpid())

void tcp_connect();

// 多进程申请客户端
int main(int argc, char *argv[])
{
	int num = 5;	// 启动 5 个子进程和服务器连接
	int i   = 0;
	
	pid_t child = 0;

	for (; i < num; i++) {
		if ((child = fork()) == 0) {
			// 子进程，连接 tcp 服务后，直接发送一条消息然后断开连接即可
			tcp_connect();
			exit(0);
		} else if (child < 0) {
			// parent
			exit(-1);
		}
	}

	// 父进程等待子进程
	for (i = 0; i < num; i++) {
		wait(NULL);
	}

	return 0;
}

void tcp_connect()
{
	int    cfd = 0;
	char   rd_buf[64] = { 0 };
	size_t rd_num = 0;

	struct sockaddr_in addr = { 0 };

	addr.sin_family = AF_INET;
	addr.sin_port   = htons(TARGET_PORT);
	addr.sin_addr.s_addr = inet_addr(TARGET_ADDR);	

	cfd = socket(AF_INET, SOCK_STREAM, 0);
	if (cfd < 0) {
		PROC_PRINT("create socket error!");
		exit(-1);
	}

	if (connect(cfd, (const struct sockaddr *)&addr, sizeof(addr)) < 0) {
		PROC_PRINT("connect server error!");
		exit(-1);
	}

	sprintf(rd_buf, "command from %d", getpid());

	// 向socket 写入指令
	if (write(cfd, rd_buf, strlen(rd_buf) + 1) <= 0) {
		PROC_PRINT("write error!");
		exit(-1);
	}

	// 读取回射的指令并且显示在标准输出
	if ((rd_num = read(cfd, rd_buf, sizeof(rd_buf))) > 0) {
		write(STDOUT_FILENO, rd_buf, rd_num);
	}

	close(cfd);
}

