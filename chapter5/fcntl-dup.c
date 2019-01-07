#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "../error-handle.h"
#include "../global.h"

BOOLEAN is_share(int oldfd, int newfd) 
{
	off_t offset[6] = { 0, 0, 0, 0, 0, 0};
	int   old_flag = 0;
	int   new_flag = 0;

	if (fcntl(oldfd, F_GETFD) < 0 || fcntl(newfd, F_GETFD) < 0) {
		err_code_exit(EINVAL);
	}

	if (oldfd == newfd) {
		return TRUE;
	}

	/* 记录当前偏移位置 */
	if ((offset[0] = lseek(oldfd, 0, SEEK_CUR)) < 0) {
		err_exit("seek error!\n");
	}

	if ((offset[1] = lseek(newfd, 0, SEEK_CUR)) < 0) {
		err_exit("seek error!\n");
	}

	/* 指针偏移到最开始，记录结果 */
	if ((offset[2] = lseek(oldfd, 0, SEEK_SET)) < 0) {
		err_exit("seek error!\n");
	}

	if ((offset[3] = lseek(newfd, 0, SEEK_CUR)) < 0) {
		err_exit("seek error!\n");
	}

	/* 指针移动到尾端，记录结果 */
	if ((offset[4] = lseek(oldfd, 0, SEEK_END)) < 0) {
		err_exit("seek error!\n");
	}

	if ((offset[5] = lseek(newfd, 0, SEEK_CUR)) < 0) {
		err_exit("seek error!\n");
	}

	/* 还原指针位置 */
	if (lseek(oldfd, offset[0], SEEK_SET) < 0) {
		err_exit("seek error!\n");
	}

	if (lseek(newfd, offset[1], SEEK_SET) < 0) {
		err_exit("seek error!\n");
	}

	/* 检测打开文件状态标识是否相同 */
	if ((old_flag = fcntl(oldfd, F_GETFL)) < 0) {
		err_exit("get flag error!\n");
	}

	if ((new_flag = fcntl(newfd, F_GETFL)) < 0) {
		err_exit("get new flag error!\n");
	}

	return (old_flag == new_flag) && (offset[2] == offset[3]) && (offset[4] == offset[5]);
}

int fcntl_dup(int oldfd) 
{
	if (oldfd < 0) {
		err_code_not_exit(EINVAL);
		return FAILED;
	}

	return fcntl(oldfd, F_DUPFD, oldfd + 1);
}

int fcntl_dup2(int oldfd, int newfd)
{
	if (oldfd < 0 || newfd < 0) {
		err_code_not_exit(EINVAL);
		return FAILED;
	}

	if (oldfd == newfd && fcntl(oldfd, F_GETFD) < 0) {
		err_code_not_exit(EBADF);
		return FAILED;
	}

	return fcntl(oldfd, F_DUPFD, newfd);
}

int main(int argc, char *argv[])
{
	int oldfd  = 0;
	int newfd  = 0;
	int newfd2 = 0;

	if ((oldfd = open(argv[1], O_RDONLY)) < 0) {
		err_exit("open failed!\n");
	}

	newfd = fcntl_dup(oldfd);
	newfd2 = fcntl_dup2(oldfd, newfd);

	if (is_share(oldfd, newfd)) {
		printf("oldfd-%d and newfd-%d is in same file!\n", oldfd, newfd);
	}

	if (is_share(newfd, newfd2)) {
		printf("newfd-%d and newfd2-%d is in same file!\n", newfd, newfd2);
	}

	printf("oldfd = %d\nnewfd = %d\nnewfd2 = %d\n", oldfd, newfd, newfd2);

	if (close(oldfd)) {
		err_exit("close old failed!\n");
	}

	if (close(newfd)) {
		err_exit("close new failed!\n");
	}

	if (close(newfd2)) {
		err_exit("close new2 failed!\n");
	}

	return 0;
}

