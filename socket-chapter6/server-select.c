#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>

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

void handle_info(int fd) 
{
	char rd_buf[64] = { 0 };
	size_t rd_size  = 0;
	unsigned int sel = 0;

	while ((rd_size = read(fd, rd_buf, 64)) > 0) {
		rd_buf[rd_size - 1] = '\0';
		if (!strcmp(rd_buf, "server quit")) {
			break;
		}

		sel = ((unsigned int)rand()) % 8;
		strcpy(rd_buf, "server:");
		strncat(rd_buf, res_list[sel], 64);
		rd_buf[63] = '\0';

		if (write(fd, rd_buf, strlen(rd_buf) + 1) < 0) {
			break;
		}
	}
}

// 该用 select 驱动的 tcp 回射服务器
int main(int argc, char *argv[])
{
	int cfds[FD_SETSIZE] = { 0 };
	fd_set all_set;				// 描述符集
	fd_set rd_set;				// 可读的文件描述符标记集合
	
	int i = 0;
	int max_index = 0;			// 当前cfds中描述符所占最大的索引

	int lfd    = 0;				// 监听 socket
	int cfd	   = 0;				// 连接描述符
	int maxfd  = 0;
	int fd_num = 0;				// 文件描述符剩余数量，select 的返回值

	struct sockaddr_in srv_addr = { 0 };
	struct sockaddr_in cli_addr = { 0 };
	socklen_t clit_len = 0;

	char buff[64]  = { 0 };
	const char *str_addr = NULL;
	int port = 0;

	srand(time(NULL));

	// create socket->bind->listen->select->accept/other
	if ((lfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("socket lfd error!\n");
		_exit(-1);
	}

	memset(&srv_addr, 0, sizeof(srv_addr));
	memset(&cli_addr, 0, sizeof(cli_addr));

	srv_addr.sin_family = AF_INET;
	srv_addr.sin_addr.s_addr = DEF_ADDR;
	srv_addr.sin_port = DEF_PORT;

	if (bind(lfd, (struct sockaddr *)&srv_addr, sizeof(srv_addr)) < 0) {
		printf("bind error!\n");
		_exit(-1);
	}

	if (listen(lfd, DEF_BLOG) < 0) {
		printf("listen error!\n");
		_exit(-1);
	}

	// 初始化文件描述符集的每个元素为 -1
	for (; i < FD_SETSIZE; i++) {
		cfds[i] = -1;
	}

	FD_ZERO(&all_set);
	FD_SET(lfd, &all_set);
	maxfd = lfd;

	for (;;) {
		rd_set = all_set;
		fd_num = select(maxfd + 1, &rd_set, NULL, NULL, NULL);
		
		// 监听描述符可读
		if (FD_ISSET(lfd, &rd_set)) {
			cfd = accept(lfd, (struct sockaddr *)&cli_addr, &clit_len);
			if (cfd < 0) {
				if (errno == EINTR) {
					// 系统中断
					continue;
				} else {
					// 异常
					printf("accept error!\n");
					break;
				}
			}

			// 保存新的连接描述符
			for (i = 0; i < FD_SETSIZE; i++) {
				if (cfds[i] == -1) {
					cfds[i] = cfd;
					break;
				}
			}

			if (i == FD_SETSIZE) {
				printf("too many clients!\n");
				break;
			}

			str_addr = inet_ntop(AF_INET, &cli_addr, buff, sizeof(buff));
			port = ntohs(cli_addr.sin_port);
			printf("connection from %s:%d\n", (str_addr ? str_addr : ""), port);

			// 向描述符集添加新的描述符
			FD_SET(cfd, &all_set);
			if (cfd > maxfd) {
				maxfd = cfd;
			}

			// 记录有效描述符所占最大索引
			if (i > max_index) {
				max_index = i;
			}

			// 如果有多个可读的描述符需要处理，那么重新进入循环进行判断
			fd_num--;
			if (fd_num <= 0) {
				continue;
			}
		}

		for (i = 0; i <= max_index; i++) {
			// 遍历保存的已连接描述符，依次进行处理
			if ((cfd = cfds[i]) < 0) {
				continue;
			}

			if (FD_ISSET(cfd, &all_set)) {
				// 处理信息
				handle_info(cfd);

				// 停止服务通知
				printf("stop server for %s:%d\n", (str_addr ? str_addr : ""), port);

				close(cfd);
				cfds[i] = -1;
				FD_CLR(cfd, &all_set);
			}

			// 处理完任务后, 检查是否有剩余的任务需要处理
			fd_num--;
			if (fd_num <= 0) {
				break;
			}
		}
	}

	close(lfd);
	return 0;
}

