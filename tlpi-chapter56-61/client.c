#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char *argv[]) 
{
	int sfd = 0;
	int rd_num = 0;
	
	char rd_buf[64] = { 0 };
	struct sockaddr addr = { 0 };

	if ((sfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
		_exit(-1);
	}

	addr.sa_family = AF_UNIX;
	strncpy(addr.sa_data, "server", sizeof(addr.sa_data) - 1);

	if (connect(sfd, &addr, sizeof(struct sockaddr)) < 0) {
		_exit(-1);
	}

	while ((rd_num = read(STDIN_FILENO, rd_buf, 64)) > 0) {
		if (write(sfd, rd_buf, rd_num) != rd_num) {
			_exit(-1);
		}

		if (rd_num > 0 && rd_num <= 64) {
			rd_buf[rd_num - 1] = '\0';
			if (!strcmp(rd_buf, "quit")) {
				break;
			}
		}
	}

	close(sfd);

	return 0;
}
