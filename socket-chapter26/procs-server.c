#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <signal.h>

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BACKLOG 10

typedef struct sockaddr SA;

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

void * handle_info(int fd) 
{
	int    port = 0;
	char   rd_buf[64] = { 0 };
	size_t rd_size = 0;

	unsigned int sel = 0;

	struct sockaddr_in addr = { 0 };
	socklen_t len = 0;

	if (getsockname(fd, (struct sockaddr *)&addr, &len) < 0) {
		printf("getsockname error!\n");
		return NULL;
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
	return NULL;
}

void sig_handle(int sig)
{
	pid_t child = 0;

	while ((child = waitpid(-1, NULL, WNOHANG)) > 0) {
		printf("process %d will exit!\n", child);
	}
}

// 多进程服务
int main(int argc, char *argv[])
{
	int cfd = 0;
	int lfd = 0;

	pid_t child = 0;	// 子进程 ID

	struct sockaddr_in svr_addr = { 0 };
	struct sockaddr_in cli_addr = { 0 };

	struct sigaction sig = { 0 };	// 处理死掉的子进程
	sigset_t sig_mask;

	sig.sa_handler = sig_handle;
	sig.sa_flags   = SA_RESTART;	// 设置可重入标志
	sigemptyset(&sig.sa_mask);

	if (sigaction(SIGCHLD, &sig, NULL) < 0) {
		printf("set signal listener error!\n");
		exit(-1);
	}

	lfd = socket(AF_INET, SOCK_STREAM, 0);
	if (lfd < 0) {
		printf("create socket error!\n");
		exit(-1);
	}

	memset(&svr_addr, 0, sizeof(svr_addr));
	memset(&cli_addr, 0, sizeof(cli_addr));

	svr_addr.sin_family = AF_INET;
	svr_addr.sin_port   = htons(9999);
	svr_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	if (bind(lfd, (SA *)&svr_addr, sizeof(svr_addr)) < 0) {
		printf("bind socket error!\n");
		exit(-1);
	}

	if (listen(lfd, BACKLOG) < 0) {
		printf("listen error!\n");
		exit(-1);
	}

	for (;;) {
		cfd = accept(lfd, NULL, NULL);
		if (cfd < 0) {
			if (errno == EINTR) {
				continue;
			} else {
				printf("accept error! -- %d\n", errno);
				exit(-1);
			}
		}

		if ((child = fork()) == 0) {
			// child process
			close(lfd);
			handle_info(cfd);

			close(cfd);
			exit(0);
		} else if (child < 0) {
			exit(-1);
		}

		close(cfd);
	}

	close(lfd);

	return 0;
}

