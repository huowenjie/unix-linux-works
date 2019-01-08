#include <sys/types.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "../error-handle.h"
#include "../global.h"

#ifndef IOV_MAX
#define IOV_MAX 16
#endif // IOV_MAX

/*
struct iovec {
	void *iov_base;
	size_t iov_len;
} 
*/

typedef unsigned char BYTE;

ssize_t readv_read(int fd, const struct iovec *iov, int iovcnt)
{
	int 	i	 	= 0;
	ssize_t rd_num 	= 0;
	ssize_t rd_tmp 	= 0;

	if (!iov) {
		err_exit("iov is null!\n");
	}

	if (iovcnt > IOV_MAX || iovcnt <= 0) {
		err_exit("iov count is out of range!\n");
	}

	if (lseek(fd, 0, SEEK_SET) < 0) {
		err_exit("seek error!\n");
	}

	for (i = 0; i < iovcnt; i++) {
		if ((rd_tmp = read(fd, iov[i].iov_base, iov[i].iov_len)) < 0) {
			err_exit("read error!\n");
		}

		rd_num += rd_tmp;
	}

	return rd_num;
}

ssize_t writev_write(int fd, const struct iovec *iov, int iovcnt)
{
	int 	i	 	= 0;
	ssize_t wr_num 	= 0;
	ssize_t wr_tmp 	= 0;

	if (!iov) {
		err_exit("iov is null!\n");
	}

	if (iovcnt > IOV_MAX || iovcnt <= 0) {
		err_exit("iov count is out of range!\n");
	}

	if (lseek(fd, 0, SEEK_SET) < 0) {
		err_exit("seek error!\n");
	}

	for (i = 0; i < iovcnt; i++) {
		if ((wr_tmp = write(fd, iov[i].iov_base, iov[i].iov_len)) < 0) {
			err_exit("read error!\n");
		}

		wr_num += wr_tmp;
	}

	return wr_num;
}

int main(int argc, char *argv[])
{
	int 	fd = 0;
	ssize_t wr_num = 0;
	ssize_t rd_num = 0;

	char str_buff1[] = "100 ret from 192.168.2.89";
	char str_buff2[] = "-100 ret from 192.168.2.89";
	char str_buff3[] = "100 ret from 192.168.2.90";
	char rd_buff1[50] = { 0 };
	char rd_buff2[50] = { 0 };
	char rd_buff3[50] = { 0 };

	struct iovec iovs[3] = { 
		{ str_buff1, strlen(str_buff1) + 1 },
		{ str_buff2, strlen(str_buff2) + 1 },
		{ str_buff3, strlen(str_buff3) + 1 }
	};

	if ((fd = open(argv[1], O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR)) < 0) {
		err_exit("open failed!\n");
	}

	wr_num = writev_write(fd, iovs, 3);

	iovs[0].iov_base = rd_buff1;
	iovs[0].iov_len  = 50;
	iovs[1].iov_base = rd_buff2;
	iovs[1].iov_len  = 50;
	iovs[2].iov_base = rd_buff3;
	iovs[2].iov_len  = 50;

	rd_num = readv_read(fd, iovs, 3);

	printf("write num is %d\nread num is %d\n", (int)wr_num, (int)rd_num);
	printf(
		"1--%s\n2--%s\n3--%s\n", 
		(char *)iovs[0].iov_base, 
		(char *)iovs[1].iov_base, 
		(char *)iovs[2].iov_base);

	if (close(fd) < 0) {
		err_exit("close error!\n");
	}

	return 0;
}

