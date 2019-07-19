#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../error-handle.h"

static void handler(int signo)
{
	char *signm = NULL;

	if (!signo) {
		printf("get empty signal!\n");
		return;
	}

	signm = strsignal(signo);
	printf("get signal -- %s\n", signm);
	
	if (signo == SIGQUIT) {
		exit(0);
	}
}

int main(int argc, char *argv[])
{
	int i = 0;
	int flag = 0;
	struct sigaction act;

	if (argc == 2) {
		flag = atoi(argv[1]);
	}

	printf("not support SIGKILL-%d, SIGSTOP-%d\n", SIGKILL, SIGSTOP);

	for (i = 1; i < 31; i++) {
		if (i == SIGKILL || i == SIGSTOP) {
			continue;
		}

		act.sa_handler = handler;
		act.sa_flags = 0;

		if (flag == 1) {
			act.sa_flags |= SA_RESETHAND;
		} else if (flag == 2) {
			act.sa_flags |= SA_NODEFER;
		} else {
			act.sa_flags = 0;
		}

		sigemptyset(&act.sa_mask);	

		if (sigaction(i, &act, NULL) < 0) {
			err_exit("register %d failed, total = 31\n", i);
		}
	}	
	
	while (1) {
		pause();
	}

	return 0;
}

