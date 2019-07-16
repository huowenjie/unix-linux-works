#include <signal.h>
#include <unistd.h>
#include <stdio.h>

static void sig_user(int);

int main(int argc, char *argv[])
{
	if (signal(SIGUSR1, sig_user) == SIG_ERR) {
		return -1;
	}

	if (signal(SIGUSR2, sig_user) == SIG_ERR) {
		return -1;
	}

	while (1) {
		pause();
	}

	return 0;
}

static void sig_user(int signo)
{
	if (signo == SIGUSR1) {
		printf("received sigusr1!\n");
	} else if (signo == SIGUSR2) {
		printf("received sigusr2!\n");
	} else {
		printf("receive other signal %d\n", signo);
	}
}

