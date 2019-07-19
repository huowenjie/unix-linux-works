#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	printf("my thread id = %ld\n", pthread_self());

	if (pthread_join(pthread_self(), NULL) != 0) {
		printf("join error!\n");
		exit(-1);
	}

	return 0;
}
