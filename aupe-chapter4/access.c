#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#define RWRWRW (S_IRWXU | S_IRWXG | S_IRWXO)

//创建两个文件
int main(int argc, char *argv[])
{
	int fd1 = 0;
	int fd2 = 0;
	
	umask(0);

	if ((fd1 = open("test1.txt", O_CREAT, RWRWRW)) < 0) {
		exit(-1);			
	}

	umask(S_IXUSR | S_IXGRP | S_IXOTH);

	if ((fd2 = open("test2.txt", O_CREAT, RWRWRW)) < 0) {
		exit(-1);
	}


	close(fd1);
	close(fd2);

	return 0;
}

