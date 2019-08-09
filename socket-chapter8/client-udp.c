#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdlib.h>

#define DEF_ADDR inet_addr("127.0.0.1")
#define DEF_PORT htons(9999)
#define DEF_BLOG 10

int main(int argc, char *argv[])
{
	int cfd = 0;
	struct sockaddr_in svr_addr = { 0 };

	fd_set fds;

	char buff[64] = { 0 };
	size_t rd_size = 0;

	// 生成基于用户数据报类型的套接字
	cfd = socket(AF_INET, SOCK_DGRAM, 0);

	memset(&svr_addr, 0, sizeof(svr_addr));
	svr_addr.sin_family = AF_INET;
	svr_addr.sin_addr.s_addr = DEF_ADDR;
	svr_addr.sin_port = DEF_PORT;

	FD_ZERO(&fds);
	FD_SET(cfd, &fds);
	FD_SET(STDIN_FILENO, &fds);

	for (;;) {
		// 目前能够从标准输入读取
		if ((rd_size = read(STDIN_FILENO, buff, 64)) <= 0) {
			printf("read error from stdin\n");
			_exit(-1);
		}

		buff[rd_size - 1] = '\0';
		if (!strcmp(buff, "quit")) {
			break;
		}

		sendto(cfd, buff, rd_size, 0, (const struct sockaddr *)&svr_addr, sizeof(svr_addr));

		if ((rd_size = recvfrom(cfd, buff, 64, 0, NULL, NULL)) == 0) {
			printf("server has been terminated!\n");
			_exit(-1);
		}

		write(STDOUT_FILENO, buff, rd_size);
	}

	return 0;
}

