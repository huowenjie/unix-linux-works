#include <sys/types.h>
#include <sys/socket.h>
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

const char *res_list[] = {
	"Hello World!\n",
	"Are you OK?\n",
	"What are you talking about?\n",
	"Oh no, I want to my uncle home!\n",
	"It's too difficult!\n",
	"I don't know, you can ask someone!\n",
	"Go go go!\n",
	"Follow me!\n",
	"Fire in the hole!\n",
	NULL
};

void handle_info(int fd, struct sockaddr_in *sa, socklen_t *len) 
{
	char rd_buf[64]  = { 0 };
	size_t rd_size   = 0;
	unsigned int sel = 0;

	const char *str_addr = NULL;
	char ad_buf[64] = { 0 };
	int port = 0;

	for (;;) {
		rd_size = recvfrom(fd, rd_buf, 64, 0, (struct sockaddr *)sa, len);
		if (rd_size < 0) {
			printf("recv error!\n");
			break;
		}

		// 显示来源消息
		str_addr = inet_ntop(AF_INET, sa, ad_buf, sizeof(ad_buf));
		port = ntohs(sa->sin_port);
		printf("receive from %s:%d, data size is %ld byte\n", 
			(str_addr ? str_addr : ""), port, rd_size);

		rd_buf[rd_size - 1] = '\0';
		if (!strcmp(rd_buf, "server quit")) {
			continue;
		}

		sel = ((unsigned int)rand()) % 8;
		strncpy(rd_buf, res_list[sel], 64);
		rd_buf[63] = '\0';

		sendto(fd, rd_buf, strlen(rd_buf) + 1, 0, (const struct sockaddr *)sa, *len);
	}
}

// 简易 udp 回显服务
int main(int argc, char *argv[])
{
	int lfd = 0;
	struct sockaddr_in svr_addr = { 0 };
	struct sockaddr_in cli_addr = { 0 };
	socklen_t len = 0;

	// 生成基于用户数据报类型的套接字
	lfd = socket(AF_INET, SOCK_DGRAM, 0);

	memset(&svr_addr, 0, sizeof(svr_addr));
	memset(&cli_addr, 0, sizeof(cli_addr));

	svr_addr.sin_family = AF_INET;
	svr_addr.sin_addr.s_addr = DEF_ADDR;
	svr_addr.sin_port = DEF_PORT;

	if (bind(lfd, (struct sockaddr *)&svr_addr, sizeof(svr_addr)) < 0) {
		printf("bind error!\n");
		_exit(-1);
	}

	handle_info(lfd, &cli_addr, &len);

	close(lfd);
	return 0;
}

