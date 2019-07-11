#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void read_file(int fd, char *buff, int size)
{
	int rd_size = 0;

	while ((rd_size = read(fd, buff, size)) > 0) {
		if (write(STDOUT_FILENO, buff, rd_size) != rd_size) {
			break;
		}
	}
}

void write_file(int fd, char *buff, int size)
{
	int rd_size = 0;

	if ((rd_size = read(STDIN_FILENO, buff, size)) > 0) {
		if (write(fd, buff, rd_size) < 0) {
			printf("fail to write!\n");
		}
	}
}

int main(int argc, char *argv[])
{
	char buff[128] = { 0 };
	int fd = 0;

	if (argc >= 2 && !strcmp(argv[1], "append")) {
		fd = open("test.txt", O_APPEND | O_RDWR);
	} else {
		fd = open("test.txt", O_RDWR);
	}

	if (fd < 0) {
		exit(-1);
	}

	printf("open test.txt successfully!\n");
	printf("fd will move to start!\n");
	printf("please start first append:\n");
	
	write_file(fd, buff, 128);
	if (lseek(fd, 0, SEEK_SET) < 0) {
		exit(-1);	
	}

	read_file(fd, buff, 128);
	
	printf("now change head of file!\n");
	if (lseek(fd, 0, SEEK_SET) < 0) {
		exit(-1);
	}
	write_file(fd, buff, 128);
	if (lseek(fd, 0, SEEK_SET) < 0) {
		exit(-1);
	}

	printf("final file is :\n");
	read_file(fd, buff, 128);

	close(fd);

	return 0;
}

