#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../error-handle.h"
#include "../global.h"

#define BUFF_LEN 64

/* 定义缓冲区和长度 */
static unsigned char buff[BUFF_LEN] = { 0 };

int main(int argc, char *argv[])
{
	int 	source_fd = 0;
	int 	dest_fd   = 0;
	ssize_t	read_num  = 0;
	ssize_t write_num = 0;
	mode_t  file_mode = (S_IRWXU & ~S_IXUSR) | (S_IRWXG & ~S_IXGRP) |
						S_IROTH;

	if (argc != 3) {
		err_exit("args num must be 2!\n");
	}

	/* 1.打开源文件 */
	if ((source_fd = open(argv[1], O_RDONLY)) < 0) {
		err_exit("open source failed!\n");
	}

	/* 2.打开目标文件 */
	if ((dest_fd = open(argv[2], O_RDWR | O_TRUNC | O_CREAT, file_mode)) < 0) {
		err_exit("open dest failed!\n");
	}

	/* 3.读取源文件，写入目标文件 */
	while ((read_num = read(source_fd, buff, BUFF_LEN)) > 0) {
		if ((write_num = write(dest_fd, buff, read_num)) < 0) {
			err_exit("write error!\n");
		}
	}

	/* 4.关闭源文件 */
	if (close(source_fd)) {
		err_exit("close source failed!\n");
	}

	/* 5.关闭目标文件 */
	if (close(dest_fd)) {
		err_exit("close dest failed!\n");
	}

	return 0;
}

