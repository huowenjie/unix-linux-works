#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include "../error-handle.h"

/**
 * 检查文件类型
 * i.普通文件-赋予所有用户读权限，查看是否有执行权限，如果有
 * 赋予该文件执行权限
 * 2.目录-直接赋予读权限和可执行权限
 */
int main(int argc, char *argv[])
{
	struct stat buf = { 0 };
	mode_t mode = 0;

	if (argc != 2) {
		err_exit("args count must be 2!\n");
	}

	if (stat(argv[1], &buf) < 0) {
		err_exit("get info error!\n");
	}

	mode = buf.st_mode;

	if (S_ISREG(mode)) {
		// 普通文件
		if ((mode & S_IXUSR) == S_IXUSR ||
			(mode & S_IXGRP) == S_IXGRP || 
			(mode & S_IXOTH) == S_IXOTH) {
			if (chmod(argv[1], S_IRUSR | S_IRGRP | S_IROTH |
							   S_IXUSR | S_IXGRP | S_IXOTH) < 0) {
				err_exit("chmod file1 error!\n");
			}
		} else {
			if (chmod(argv[1], S_IRUSR | S_IRGRP | S_IROTH) < 0) {
				err_exit("chmod file2 error!\n");
			}
		}
	} else if (S_ISDIR(mode)) {
		// 目录文件
		if (chmod(argv[1], S_IRUSR | S_IRGRP | S_IROTH |
						   S_IXUSR | S_IXGRP | S_IXOTH) < 0) {
			err_exit("chmod dir error!\n");
		}
	} else {
		printf("unknow file type!\n");
	}

	return 0;
}

