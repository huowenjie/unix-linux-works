#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

static int error_code = 0;

static void * handle(void *args) 
{
	printf("This is child thread! start sleeping...\n");
	sleep(3);
	printf("Child thread exit!\n");
	
	error_code = 100;

	return &error_code;
}

int main(int argc, char *argv[])
{
	pthread_t pid;
	int *ret = NULL;

	if (pthread_create(&pid, NULL, handle, NULL) != 0) {
		printf("create thread error!\n");
		exit(-1);
	}

	printf("create thread successfully!\n");

	if (pthread_join(pid, (void **)&ret) != 0) {
		printf("join error!\n");
		exit(-1);
	}

	printf("Main thread end! subthread ret = %d\n", *ret);

	return 0;
}
