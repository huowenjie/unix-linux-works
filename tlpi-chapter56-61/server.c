#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

void create_server()
{
	int sfd = 0;
	int cfd = 0;
	int rd_num = 0;
	int exit_flag = 0;

	char rd_buf[64] = { 0 };
	struct sockaddr addr = { 0 };

	if ((sfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
		_exit(-1);
	}

	addr.sa_family = AF_UNIX;
	strncpy(addr.sa_data, "server", sizeof(addr.sa_data) - 1);

	if (bind(sfd, &addr, sizeof(struct sockaddr)) < 0) {
		_exit(-1);
	}

	if (listen(sfd, 5) < 0) {
		_exit(-1);
	}

	while (!exit_flag) {
		// 如果没有消息将会阻塞
		if ((cfd = accept(sfd, NULL, NULL)) < 0) {
			_exit(-1);
		}
		
		// 从流中读取数据并打印
		while ((rd_num = read(cfd, rd_buf, 64)) > 0) {
			if (write(STDOUT_FILENO, rd_buf, rd_num) != rd_num) {
				_exit(-1);
			}

			if (rd_num > 0 && rd_num <= 64) {
				rd_buf[rd_num - 1] = '\0';
				if (!strcmp(rd_buf, "server stop")) {
					exit_flag = 1;
					break;
				}
			}
		}

		if (close(cfd) < 0) {
			_exit(-1);
		}
	}

	close(sfd);

	// 移除 socket
	if (remove("server") < 0) {
		_exit(-1);
	}
}

// 创建一个守护进程，并且捕获信号退出
int main(int argc, char *argv[]) 
{
	pid_t pid = 0;

	// 确保进程具有文件访问的权限
	umask(0);

	if ((pid = fork()) > 0) {
		// 结束父进程
		_exit(0);
	} else if (pid < 0) {
		_exit(-1);	
	}

	// 创建一个会话
	setsid();

	// 由于是测试程序，故不需要将工作目录设置置根
	if (chdir(".") < 0) {
		_exit(-1);
	}

	// 再次终止父进程，确保该会话不是首会话
	if ((pid = fork()) > 0) {
		_exit(0);
	} else if (pid < 0) {
		_exit(-1);
	}

	// ----------------------------------------------------

	// 创建服务
	create_server();

	printf("server will be ending!\n");
	return 0;
}

