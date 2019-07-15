#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

/**
 * 统计各种类型文件数量
 */
struct statistic_st
{
	long file;
	long dir;
	long max_depth;
	long cur_depth;
};

/**
 * 缓冲区
 */
struct name_buff_st
{
	char  *buff;
	size_t size;
};

/**
 * 判断当前路径名是否存在
 */
int isvalid(const char *path)
{
	if (!path || !path[0]) {
		return 0;
	}

	if (access(path, F_OK) < 0) {
		return 0;
	}
	
	return 1;
}

/**
 * 1.要检查当前要读取路径的有效性
 * 2.首先处理当前目录的情况，然后获取到当前目录的全路径
 * 3.获取到目标目录的全路径后，调用 opendir,readdir 遍历该目录的文件列表
 * --遍历时遇到普通文件，通过 printf 输出
 * --遍历过程中遇到 dir 类型，递归执行 3
 */

static struct statistic_st FILE_INFO = { 0 };	// 文件统计信息
static struct name_buff_st NAME_BUFF = { 0 };	// 文件名缓冲区

int show_tree()
{
	struct stat buf = { 0 };
	size_t cur_size = 0;

	struct dirent *dr = NULL;
	DIR *dir = NULL;

	int ret = 0;
	int i = 0;

	if (!isvalid(NAME_BUFF.buff)) {
		return -1;
	}

	if (stat(NAME_BUFF.buff, &buf) < 0) {
		return -1; // 发生错误，返回 -1
	}

	// 检查是否为普通文件
	if (!S_ISDIR(buf.st_mode)) {
		FILE_INFO.file++;
		return 0;
	}
	
	// 拓展内存容量，然后在目录后面加一个 /
	cur_size = strlen(NAME_BUFF.buff);
	if (cur_size + NAME_MAX + 2 > NAME_BUFF.size) {
		NAME_BUFF.size = cur_size + cur_size + NAME_MAX + 2;
		if ((NAME_BUFF.buff = realloc(NAME_BUFF.buff, NAME_BUFF.size)) == NULL) {
			printf("Expand mem error!\n");
		}
	}

	if (NAME_BUFF.buff[cur_size - 1] != '/') {
		NAME_BUFF.buff[cur_size++] = '/';
		NAME_BUFF.buff[cur_size] = 0;
	}

	if (FILE_INFO.dir == 0) {
		printf("%s\n", NAME_BUFF.buff);
	}

	FILE_INFO.dir++;
	
	if ((dir = opendir(NAME_BUFF.buff)) == NULL) {
		printf("Open dir error!\n");
		return -1;
	}
	
	while ((dr = readdir(dir)) != NULL) {
		// 忽略 . 和 ..
		if (!strcmp(dr->d_name, ".") || !strcmp(dr->d_name, "..")) {
			continue;
		}

		/**
		 * 根据当前目录深度在前面加空格
		 */ 
		printf("|");
		for (i = 0; i < FILE_INFO.cur_depth; i++) {
			printf("    |");
		}
	
		printf("---%s\n", dr->d_name);

		// 如果遍历到文件夹，增加深度
		if (dr->d_type == DT_DIR) {
			FILE_INFO.cur_depth++;
		}

		// 统计最大深度
		if (FILE_INFO.cur_depth > FILE_INFO.max_depth) {
			FILE_INFO.max_depth = FILE_INFO.cur_depth;
		}

		// 每遍历到一个文件，则重新执行本方法，同时要将文件名拼接
		strcpy(&(NAME_BUFF.buff[cur_size]), dr->d_name);
		if ((ret = show_tree()) != 0) {
			break;				
		}

		// 减少深度
		if (dr->d_type == DT_DIR) {
			FILE_INFO.cur_depth--;
		}
	}

	if (closedir(dir) < 0) {
		printf("close dir error!\n");
	}
	
	return 0;
}

/**
 * 显示目录树状结构图
 */
int main(int argc, char *argv[])
{
	char *src 		= NULL;
	char cur_path[] = ".";
	long cur_size 	= 0;
	long dir_num 	= 0;

	// 当前目录
	// 如果要搜索当前路径，需要将 . 或者 ./ 做转换
	if (argc == 1) {
		NAME_BUFF.size = strlen(cur_path) + 1;
		src = cur_path;
	} else if (argc == 2) {
		NAME_BUFF.size = strlen(argv[1]) + 1;
		src = argv[1];
	} else {
		printf("The count of args is not correct!\n");
		return -1;
	}

	// 为文件名缓冲区分配内存
	NAME_BUFF.buff = (char *)malloc(NAME_BUFF.size);
	if (!NAME_BUFF.buff) {
		printf("Mem alloc for buff error!\n");
		return -1;
	}

	memset(NAME_BUFF.buff, 0, NAME_BUFF.size);
	strcpy(NAME_BUFF.buff, src);

	show_tree();

	dir_num = (FILE_INFO.dir - 1) < 0 ? 0 : (FILE_INFO.dir - 1);

	printf("\nvisit dir count:  %ld\n", FILE_INFO.dir);
	printf("child dir count:  %ld\n",   dir_num); 
	printf("child file count: %ld\n",   FILE_INFO.file);
	printf("the num of child: %ld\n",   (FILE_INFO.file + dir_num));
	printf("max depth of dir: %ld\n\n", FILE_INFO.max_depth);

	if (NAME_BUFF.buff) {
		free(NAME_BUFF.buff);
	}

	NAME_BUFF.size = 0;

	return 0;
}

