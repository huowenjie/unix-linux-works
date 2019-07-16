#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef BUF_SIZE
#define BUF_SIZE 128
#endif // BUF_SIZE

#define RWRWR (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH)

int check_type(const char *path)
{
	struct stat buf = { 0 };
	
	if (!path || !path[0]) {
		return -1;
	}

	if (lstat(path, &buf) < 0) {
		return -1;
	}

	// 不是普通文件不能复制
	if (!S_ISREG(buf.st_mode)) {
		return -1;
	}

	return 0;
}

int main(int argc, char *argv[])
{
	int wr_fd = 0;
	int rd_fd = 0;

	static char   buff[BUF_SIZE]   	= { 0 };
	static char   wr_buff[BUF_SIZE]	= { 0 };
	
	size_t rd_size = 0;
	int    i,j 	   = 0;

	if (argc != 3) {
		printf("args num is error!\n");
		exit(-1);
	}

	if (check_type(argv[1]) < 0) {
		printf("Not support filetype!\n");
		exit(-1);
	}

	if ((rd_fd = open(argv[1], O_RDONLY)) < 0) {
		printf("open source file error!\n");
		exit(-1);
	}

	if ((wr_fd = open(argv[2], O_CREAT | O_RDWR | O_TRUNC, RWRWR)) < 0) {
		printf("open destination error!\n");
		exit(-1);
	}

	while ((rd_size = read(rd_fd, buff, BUF_SIZE)) > 0) {
		// 拷贝不是空洞的内容
		if (write(wr_fd, buff, rd_size) != rd_size) {
			printf("write data error!\n");
			exit(-1);
		}
	}

	close(wr_fd);
	close(rd_fd);

	return 0;
}

