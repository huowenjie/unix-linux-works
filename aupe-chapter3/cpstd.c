#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 128

int main(int argc, char *argv[])
{
	int fd = 0;
	int rd_size = 0;
	char buff[BUFFER_SIZE] = { 0 };
	const char *end_flag = "exit";

	if (argc != 2) {
		printf("must 2 args!\n");
		exit(-1);
	}

	if ((fd = open(argv[1], O_CREAT | O_TRUNC | O_WRONLY)) < 0) {
		printf("fail to create file!\n");
		exit(-1);
	}

	while ((rd_size = read(STDIN_FILENO, buff, BUFFER_SIZE)) > 0) {
		if (!memcmp(buff, end_flag, strlen(end_flag))) {
			break;
		}

		if (write(fd, buff, rd_size) != rd_size) {
			printf("write error!\n");
			break;
		}
	}

	if (rd_size < 0) {
		printf("read error!\n");
	}

	close(fd);

	return 0;
}
