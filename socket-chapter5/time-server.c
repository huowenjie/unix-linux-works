#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	int lfd = 0;	// 监听 socket
	int cfd = 0;	// 连接 socket

	struct sockaddr_in saddr;	// ipv4 服务器地址信息
	struct sockaddr_in caddr;	// ipv4 客户端地址信息
	socklen_t clen = 0;			// 客户端地址信息长度

	const char *str_addr = NULL;
	unsigned int port = 0;
	char buff[64] = { 0 };

	if ((lfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		_exit(-1);
	}

	memset(&saddr, 0, sizeof(saddr));
	memset(&caddr, 0, sizeof(caddr));

	saddr.sin_family = AF_INET;
	saddr.sin_port   = htons(9999);
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);

	// 绑定 socket
	if (bind(lfd, (const struct sockaddr *)&saddr, sizeof(saddr)) < 0) {
		printf("bind error!\n");
		_exit(-1);
	}

	// 监听 socket
	if (listen(lfd, 10) < 0) {
		printf("listen error!\n");
		_exit(-1);
	}

	for (;;) {
		// 等待客户端连接
		cfd = accept(lfd, (struct sockaddr *)&caddr, &clen);
		if (cfd < 0) {
			printf("accept error!\n");
			_exit(-1);
		}

		str_addr = inet_ntop(AF_INET, &caddr, buff, sizeof(buff));
		port = ntohs(caddr.sin_port);
		printf("connection from %s:%d\n", (str_addr ? str_addr : ""), port);

		if (close(cfd) < 0) {
			_exit(-1);
		}
	}

	close(lfd);
	return 0;
}
