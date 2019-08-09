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
#define PROCNUM 5

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

static pid_t *pids = NULL;

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

	printf("A connect from %s:%d, and pid = %d\n", rd_buf, port, getpid());

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

// 不安全的进程池服务
pid_t child_main(int index, int lfd)
{
	int cfd = 0;

	struct sockaddr_in cli_addr = { 0 };
	memset(&cli_addr, 0, sizeof(cli_addr));

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

		handle_info(cfd);
		close(cfd);
	}

	close(lfd);

	return getpid();
}

// 创建子进程
pid_t create_child(int index, int lfd)
{
	pid_t pid = 0;

	if ((pid = fork()) > 0) {
		return pid;
	}

	printf("process %d is created!\n", getpid());
	return child_main(index, lfd);
}

// 中断处理信号
void sig_int(int sig)
{
	int i = 0;
	pid_t child = 0;

	for (; i < PROCNUM; i++) {
		kill(pids[i], SIGTERM);
	}

	while ((child = wait(NULL)) > 0) {
		printf("process %d will be exited!\n", child);
	}

	exit(0);
}

int main(int argc, char *argv[])
{
	int lfd = 0;
	int i   = 0;

	struct sockaddr_in svr_addr = { 0 };

	lfd = socket(AF_INET, SOCK_STREAM, 0);
	if (lfd < 0) {
		printf("create socket error!\n");
		exit(-1);
	}

	memset(&svr_addr, 0, sizeof(svr_addr));

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

	pids = malloc(PROCNUM * sizeof(pid_t));
	if (!pids) {
		printf("malloc error!\n");
		exit(-1);
	}

	// 直接创建 5 个子进程
	for (; i < PROCNUM; i++) {
		pids[i] = create_child(i, lfd);
	}

	signal(SIGINT, sig_int);

	for (;;) {
		pause();
	}

	close(lfd);
	return 0;
}

