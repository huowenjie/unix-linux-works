#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "../error-handle.h"
#include "../global.h"

/* 缓冲区长度 */
#define BUFF_LEN 			64

/*
 * 输出模式 out_mode
 */
#define MODE_STD  			0 /* 标准输出模式 */
#define MODE_FILE 			1 /* 文件输出模式 */
#define MODE_FILE_APPEND 	2 /* 文件追加模式 */

/*
 * 缓冲区
 */
static char buff[BUFF_LEN] = { 0 };

/*
 * 当前的输出模式
 */
static int out_mode = MODE_STD;

/*
 * 获取选项
 */
void get_options(const OPTION *opt)
{
	const char *str_opt = option_get_opt(opt, 0);
	const char *str_val = option_get_val(opt, 0);

	if (!str_opt) {
		if (str_val) {
			/* 文件模式 */	
			out_mode = MODE_FILE;
		} else {
			/* 标准输出模式 */
			out_mode = MODE_STD;
		}
	} else {
		if (!strcmp(str_opt, "a")) {
			if (!str_val) {
				err_exit("please enter filename!\n");
			}

			/* 文件追加模式 */
			out_mode = MODE_FILE_APPEND;
		} else {
			err_exit("unknow arg %s\n", str_opt);
		}
	}
}

void io_handle(int fd_in, int fd_out)
{
	ssize_t num_rd = 0;

	while ((num_rd = read(fd_in, buff, BUFF_LEN)) > 0) {
		if (!strncmp(buff, "quit@quit", 9)) {
			break;
		}

		if (write(fd_out, buff, num_rd) != num_rd) {
			err_exit("write buff error!\n");
		}
	}
}

int open_file(const char *file_name)
{
	int fd_out;

	if (!file_name || !file_name[0]) {
		return STDOUT_FILENO;
	}

	switch (out_mode) {
		case MODE_STD:
			fd_out = STDOUT_FILENO;
			break;
		
		case MODE_FILE:
			fd_out = open(
				file_name, O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
			break;

		case MODE_FILE_APPEND:
			fd_out = open(
				file_name, O_CREAT | O_WRONLY | O_APPEND, S_IRUSR | S_IWUSR);
			break;

		default:
			fd_out = STDOUT_FILENO;
			break;
	}

	return fd_out;
}

void close_file(int fd) {
	if (close(fd)) {
		err_exit("close file failed!\n");
	}
}

int main(int argc, char *argv[])
{
	int    fd_out = STDOUT_FILENO;
	OPTION *opt   = option_new(argc, argv);

	if (opt) {
		get_options(opt);
		fd_out = open_file(option_get_val(opt, 0));
	}

	io_handle(STDIN_FILENO, fd_out);

	close_file(fd_out);
	close_file(STDIN_FILENO);
	option_free(opt);

	return 0;
}

