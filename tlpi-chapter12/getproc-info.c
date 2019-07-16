#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../error-handle.h"

// 进程目录
static const char *proc_path = "/proc/";

// 查看字符串是否全部为数字
int is_digit(const char *str) 
{
	const char *cursor = NULL;
	char c = '\0';

	if (!str || !str[0]) {
		return 0;
	}

	for (cursor = str; (c = *cursor) != '\0'; cursor++) {
		if (c < '0' || c > '9') {
			return 0;
		}
	}

	return 1;
}

// 根据 key 显示其后面的内容
char * show_info(const char *key, char *buff)
{
	size_t key_len  = 0;
	size_t buff_len = 0;
	char  *cursor   = NULL;
	char  *end_ptr  = NULL;
	int    i, j		= 0;

	static char ret_buff[128] = { 0 };

	if (!key || !key[0]) {
		return NULL;
	}

	if (!buff || !buff[0]) {
		return NULL;
	}

	key_len  = strlen(key);
	buff_len = strlen(buff);

	if (buff_len <= key_len) {
		return NULL;
	}

	cursor = buff;

	// 第一次循环，去除字符串前面的空格和制表符
	for (i = key_len; i < buff_len; i++) {
		if (cursor[i] == ' ' ||
			cursor[i] == '\t') {
			continue;
		} else {
			break;
		}
	}

	end_ptr = &cursor[i];

	// 第二次循环，访问到字符串末尾或碰到换行符时结束并将字符串末尾
	// 修改为'\0'
	for (; i < buff_len; i++, j++) {
		if (cursor[i] == '\n') {
			break;
		}

		ret_buff[j] = end_ptr[j];
	}

	if (ret_buff[j] != '\0') {
		ret_buff[j] = '\0';
	}

	return ret_buff;
}

int read_file(const char *path)
{
	int fd = 0;
	int ret = 0;

	int rd_count = 0;
	static char rd_buff[256] = { 0 };
	
	char *sub_str = NULL;
	char *pid     = NULL;
	char *uid	  = NULL;

	fd = open(path, O_RDONLY);
	
	do {
		if (fd < 0) {
			ret = -1;
			break;
		}

		rd_count = read(fd, rd_buff, 256);
	   	if (rd_count > 0 && rd_count <= 256) {
			rd_buff[rd_count - 1] = '\0';
		} else {
			ret = -1;
			break;
		}

		printf("path:%s, ", path);

		// PID
		sub_str = strstr(rd_buff, "Pid:");
		pid = show_info("Pid:", sub_str);

		printf("Pid = %s, ", pid ? pid : "");

		// Uid
		sub_str = strstr(rd_buff, "Uid:");
		uid = show_info("Uid:", sub_str);

		printf("UID = %s\n", uid ? uid : "");
		
	} while (0);

	close(fd);

	return ret;
}

void show_proc_list()
{
	static char full_path[256] = { 0 };

	DIR *proc_dir = NULL;
	struct dirent *proc_drt = NULL;

	if ((proc_dir = opendir(proc_path)) == NULL) {
		err_exit("Open %s failed!\n", proc_path);
	}

	// 遍历该文件夹下所有进程文件
	while ((proc_drt = readdir(proc_dir)) != NULL) {
		if (!strcmp(proc_drt->d_name, ".")) {
			continue;
		}

		if (!strcmp(proc_drt->d_name, "..")) {
			continue;
		}

		if (!is_digit(proc_drt->d_name)) {
			continue;
		}

		// 拼接文件名
		strcpy(full_path, proc_path);
		strcat(full_path, proc_drt->d_name);
		strcat(full_path, "/status");
		if (read_file(full_path) < 0) {
			err_exit("Read file %s failed!\n", full_path);
		}	
	}

	if (closedir(proc_dir) < 0) {
		err_exit("Close %s failed!\n", proc_path);
	}
}

int main(int argc, char *argv)
{
	show_proc_list();
	return 0;
}

