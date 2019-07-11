#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RWRWR (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH)

const char *BUFF = "12345678";

// 创建一个包含空洞的文件
int main(int argc, char *argv[])
{
	int fd = 0;
	size_t wr_size = strlen(BUFF);

	if ((fd = open("cavity.txt", O_CREAT | O_TRUNC | O_RDWR, RWRWR)) < 0) {
		exit(-1);
	}

	if (write(fd, BUFF, wr_size) != wr_size) {
		printf("Write fist region error!\n");
		exit(-1);
	}

	if (lseek(fd, 8, SEEK_CUR) < 0) {
		printf("Seek error!\n");
		exit(-1);
	}

	if (write(fd, BUFF, wr_size) != wr_size) {
		printf("Write second region error!\n");
		exit(-1);
	}

	close(fd);

	return 0;
}

