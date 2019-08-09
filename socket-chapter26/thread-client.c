#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	int cfd = 0;
	struct sockaddr_in caddr = { 0 };
	const char *host = "127.0.0.1";
	unsigned int port = 9999;

	char buff[64]  = { 0 };
	size_t rd_size = 0;

	// 关于 select 的一些参数
	int maxfd = 0;
	fd_set fds;

	// 标准输入结束标志
	int stdineof = 0;

	// 初始化 fds
	FD_ZERO(&fds);

	if (argc == 2) {
		host = argv[1];
	} else if (argc == 3) {
		host = argv[1];
		port = atoi(argv[2]);
	}

	if ((cfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		_exit(-1);
	}

	memset(&caddr, 0, sizeof(caddr));
	caddr.sin_family = AF_INET;
	caddr.sin_port   = htons(port);
	caddr.sin_addr.s_addr = inet_addr(host);

	if (caddr.sin_addr.s_addr == INADDR_NONE) {
		printf("INVALID ADDR\n");
		_exit(-1);
	}

	// 连接服务器
	if (connect(cfd, (const struct sockaddr *)&caddr, sizeof(caddr)) < 0) {
		printf("connect error!\n");
		_exit(-1);
	}
	
	printf("connected to server! please enter your command:\n");

	for (;;) {
		FD_SET(cfd, &fds);

		if (!stdineof) {
			FD_SET(STDIN_FILENO, &fds);
		}

		maxfd = ((STDIN_FILENO > cfd) ? STDIN_FILENO : cfd) + 1;
		select(maxfd, &fds, NULL, NULL, NULL);

		if (FD_ISSET(cfd, &fds)) {
			// 目前能够从 socket 读取
			if ((rd_size = read(cfd, buff, 64)) == 0) {
				if (stdineof) {
					printf("server has been terminated!\n");
					break;
				} else {
					printf("server has been terminated prematurely!\n");
					_exit(-1);
				}
			}

			write(STDOUT_FILENO, buff, rd_size);
		}

		if (FD_ISSET(STDIN_FILENO, &fds)) {
			// 目前能够从标准输入读取
			if ((rd_size = read(STDIN_FILENO, buff, 64)) == 0) {
				stdineof = 1;
				shutdown(cfd, SHUT_WR);
				FD_CLR(STDIN_FILENO, &fds);
				printf("------std in end!-------\n");
				continue;
			}

			buff[rd_size - 1] = '\0';
			if (!strcmp(buff, "quit")) {
				break;
			}

			write(cfd, buff, rd_size);
		}
	}

	close(cfd);

	return 0;
}

