#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "../error-handle.h"

/*
 * open 属性加入 O_APPEND ，自动将 lseek(fd, 0, SEEK_END) 和
 * write 函数纳入同一原子操作 
 */
int main(int argc, char *argv[])
{
	int 	open_fd = 0;
	size_t 	size_w 	= 0;

	if (argc != 3) {
		err_exit("args num is error!\n");
	}

	if ((open_fd = open(argv[1], O_WRONLY | O_APPEND)) < 0) {
		err_exit("open failed!\n");
	}
	
	size_w = strlen(argv[2]);
	if (lseek(open_fd, 0, SEEK_SET)) {
		err_exit("seek failed!\n");
	}

	if (write(open_fd, argv[2], size_w) != size_w) {
		err_exit("write error!\n");
	}

	if (close(open_fd)) {
		err_exit("close failed!\n");
	}

	return 0;
}

