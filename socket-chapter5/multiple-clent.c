#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
	int i = 0;
	int sfd[5] = { 0 };
	const char *def_ip = "127.0.0.1";

	char comm[16] = "time";
	char buff[64] = { 0 };
	int  rd_size  = 0;

	struct sockaddr_in addr;

	def_ip = (argc == 2) ? argv[1] : def_ip;

	for (i = 0; i < 5; i++) {
		if ((sfd[i] = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
			_exit(-1);
		}

		addr.sin_family = AF_INET;
		addr.sin_port = htons(9999);
		addr.sin_addr.s_addr = inet_addr(def_ip);

		if ((connect(sfd[i], (struct sockaddr *)&addr, sizeof(addr))) < 0) {
			printf("connect error! ----%d\n", i);
			_exit(-1);
		}

		if (write(sfd[i], comm, 5) == 5) {
			if ((rd_size = read(sfd[i], buff, 64)) > 0) {
				buff[rd_size - 1] = '\0';
				printf("%d response: %s\n", i, buff);
			}
		}
	}

	_exit(0);

	return 0;
}

