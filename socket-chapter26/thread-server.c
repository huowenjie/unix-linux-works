#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#define DEF_ADDR inet_addr("127.0.0.1")	// 默认地址为本机地址
#define DEF_PORT htons(9999)			// 默认端口号为 9999

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

void * handle_info(void *cfd) 
{
	int    fd = 0;
	int    port = 0;
	char   rd_buf[64] = { 0 };
	size_t rd_size = 0;
	
	
	unsigned int sel = 0;

	struct sockaddr_in addr = { 0 };
	socklen_t len = 0;

	if (!cfd) {
		return NULL;
	}

	fd = *((int *)cfd);

	if (getsockname(fd, (struct sockaddr *)&addr, &len) < 0) {
		printf("getsockname error!\n");
		goto end;
	}

	port = ntohs(addr.sin_port);
	inet_ntop(AF_INET, &addr, rd_buf, sizeof(addr));

	printf("A connect from %s:%d\n", rd_buf, port);

	while ((rd_size = read(fd, rd_buf, 64)) > 0) {
		rd_buf[rd_size - 1] = '\0';
		if (!strcmp(rd_buf, "server quit")) {
			break;
		}

		sel = ((unsigned int)rand()) % 8;
		strncpy(rd_buf, res_list[sel], 64);
		rd_buf[63] = '\0';

		if (write(fd, rd_buf, strlen(rd_buf) + 1) < 0) {
			break;
		}
	}

	printf("the task end!\n");

end:

	close(fd);
	free(cfd);

	return NULL;
}

// 迭代型 tcp 服务
int main(int argc, char *argv[]) 
{
	int cfd  = 0;
	int lfd  = 0;
	int *pfd = NULL;

	struct sockaddr_in addr = { 0 };
	pthread_t task_thread;

	// 创建随机数种子
	srand(time(NULL));

	// 创建监听 socket
	if ((lfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("get cfd error! -- %d\n", errno);
		_exit(-1);
	}

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = DEF_ADDR;
	addr.sin_port = DEF_PORT;

	if (bind(lfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) < 0) {
		printf("bind error! -- %d\n", errno);
		_exit(-1);
	}

	if (listen(lfd, DEF_BLOG) < 0) {
		printf("listen error!\n");
		_exit(-1);
	}

	for (;;) {
		cfd = accept(lfd, NULL, NULL);
		if (cfd < 0) {
			// 系统中断
			if (errno == EINTR) {
				continue;
			} else {
				printf("accept error! -- %d\n", errno);
				break;
			}
		}

		pfd = (int *)malloc(sizeof(int));
		if (!pfd) {
			printf("malloc error!\n");
			_exit(-1);
		}

		// 创建一个子线程处理任务
		*pfd = cfd;
		pthread_create(&task_thread, NULL, handle_info, (void *)pfd);
	}

	close(lfd);

	return 0;
}

