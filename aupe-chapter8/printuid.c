#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	char *login_name = getlogin();

	if (!login_name) {
		printf("get login error!\n");
	}

	printf("login name is %s\n", login_name);
	printf("real uid = %d, effective uid = %d\n", getuid(), geteuid());

	sleep(3);
	return 0;
}

