#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

void check_type(const char *path)
{
	struct stat buf = { 0 };

	if (!path || !path[0]) {
		return;
	}

	if (stat(path, &buf) < 0) {
		printf("get info error!\n");
		return;
	}

	// 测试文件类型
	if (S_ISREG(buf.st_mode)) {
		printf("It is a regular file.\n");
	} else if (S_ISDIR(buf.st_mode)) {
		printf("It is directory.\n");
	} else {
		printf("Unknow file type.\n");
	}

	printf("uid = %d, gid = %d\n", buf.st_uid, buf.st_gid);
}

int main(int argc, char *argv[])
{
	int i = 0;

	if (argc < 2) {
		return 0;
	}

	for (i = 1; i < argc; i++) {
		printf("-----------start check %s-----------\n", argv[i]);
		check_type(argv[i]);
	}
	
	return 0;
}

