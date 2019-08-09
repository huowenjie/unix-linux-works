#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	int cfd = 0;
	struct sockaddr_in caddr;
	const char *host = "127.0.0.1";
	unsigned int port = 9999;

	char buff[64]  = { 0 };
	size_t rd_size = 0;

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
	while ((rd_size = read(STDIN_FILENO, buff, 64)) > 0) {
		if (write(cfd, buff, rd_size) != rd_size) {
			_exit(-1);
		}

		buff[rd_size - 1] = '\0';
		if (!strcmp(buff, "quit")) {
			break;
		}

		if ((rd_size = read(cfd, buff, 64)) > 0) {
			write(STDOUT_FILENO, buff, rd_size);
		} else {
			printf("server has been terminated!\n");
			break;
		}
	}

	close(cfd);

	return 0;
}

