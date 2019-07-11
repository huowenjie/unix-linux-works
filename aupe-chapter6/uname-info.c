#include <sys/utsname.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include "../error-handle.h"

int main(int argc, char *argv[])
{
	struct utsname buf;
	struct tm *pt = NULL;
	time_t t = 0;
	char   buff[128] = { 0 };

	if (uname(&buf) < 0) {
		err_exit("uname failed!\n");
	}

	printf("sysname--%s\n",  buf.sysname);
	printf("nodename--%s\n", buf.nodename);
	printf("release--%s\n",  buf.release);
	printf("version--%s\n",  buf.version);
	printf("machine--%s\n",  buf.machine);

	// time
	time(&t);
	pt = localtime(&t);
	if (strftime(buff, sizeof(buff), "%Y 年 %m 月 %d 日 %X %Z", pt) == 0) {
		err_exit("buff size not enough!\n");
	}

	printf("time(&t) t = %ld\n", t);
	printf("%s\n", buff);

	return 0;
}

