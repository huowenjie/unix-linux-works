#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "../error-handle.h"
#include "../global.h"

int main(int argc, char *argv[])
{
	int fd1 = open(argv[1], O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	int fd2 = dup(fd1);
	int fd3 = open(argv[1], O_RDWR);

	write(fd1, "Hello,", 6);
	write(fd2, "world", 6);
	lseek(fd2, 0, SEEK_SET);
	write(fd1, "HELLO,", 6);
	write(fd3, "Gidday", 6);

	return 0;
}
