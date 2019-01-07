#include <sys/types.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>
#include "../error-handle.h"
#include "../global.h"

/*
struct iovec {
	void *iov_base;
	size_t iov_len;
} 
*/

typedef unsigned char BYTE;

ssize_t readv_read(int fd, const struct iovec *iov, int iovcnt)
{
	int     i 			= 0;
	ssize_t rd_num 		= 0;
	size_t  iov_size 	= 0;
	BYTE 	*buff_iov 	= NULL;
	BYTE	*buff_data	= NULL;

	if (iovcnt > IOV_MAX) {
		err_exit("iov count is out of range!\n");
	}

	for (i = 0; i < iovcnt; i++) {
		iov_size = sizeof(iovec);
		buff_iov = (BYTE *)malloc(iov_size);

		if (!buff_iov) {
			err_exit("null pointer!\n");
		}

		memset(buff_iov, 0, iov_size);
		if (read(fd, buff_iov, iov_size) != )
	}
	
	return 0;
}

ssize_t writev_write(int fd, const struct iovec *iov, int iovcnt)
{
	return 0;
}

int main(int argc, char *argv[])
{
	return 0;
}

