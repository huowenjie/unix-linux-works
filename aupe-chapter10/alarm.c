#include <signal.h>
#include <unistd.h>
#include <stdio.h>

static void sig_alrm(int signo)
{
	printf("Sleep end! -- > signo = %d\n", signo);
}

static unsigned int sleep1(unsigned int second)
{
	if (signal(SIGALRM, sig_alrm) == SIG_ERR) {
		return -1;
	}

	alarm(second);

	pause();

	return alarm(0);
}

// sleep
int main(int argc, char *argv[])
{
	unsigned int second = sleep(5);
	printf("prev sleep time = %d\n", second);

	return 0;
}

