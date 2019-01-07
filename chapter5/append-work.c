#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "../error-handle.h"

static unsigned char buff[1] = { 10 };

/*
 */
int main(int argc, char *argv[])
{
	int		i 		= 0;
	int		wr_num	= 0;
	int 	open_fd = 0;
	int 	flag	= 0;
	

	if (argc < 3 || argc > 4) {
		err_exit("args num is error!\n");
	}

	flag = O_CREAT | O_WRONLY | O_APPEND;
	
	if ((argc == 4 && argv[3][0] == 'x')) {
		flag &= (~O_APPEND);
	}

	if ((open_fd = open(argv[1], flag, S_IRWXU)) < 0) {
		err_exit("open failed!\n");
	}

	wr_num = atoi(argv[2]);

	if ((flag & O_APPEND) != O_APPEND) {	
		for (i = 0; i < wr_num; i++) {
			if (lseek(open_fd, 0, SEEK_END) < 0) {
				err_exit("seek failed!\n");
			}
		
			if (write(open_fd, buff, 1) != 1) {
				err_exit("write error!\n");
			}
		}
	} else {
		for (i = 0; i < wr_num; i++) {
			if (write(open_fd, buff, 1) != 1) {
				err_exit("write error!\n");
			}
		}	
	}

	if (close(open_fd)) {
		err_exit("close failed!\n");
	}

	return 0;
}

