#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	if (access("test1.txt", F_OK) < 0) {
		printf("test1.txt not exsit\n");
		return -1;
	}

	if (access("test2.txt", F_OK) < 0) {
		printf("test2.txt not exit");
		return -1;
	}

	chmod("test1.txt", 0755);
	chmod("test2.txt", 0777);

	return 0;
}

