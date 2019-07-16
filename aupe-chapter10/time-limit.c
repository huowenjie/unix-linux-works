#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <setjmp.h>

static jmp_buf env_alrm;
static void sig_alrm(int signo);

int main(int argc, char *argv[])
{
	char rd_buf[16] = { 0 };
	int  rd_num = 0;

	if (signal(SIGALRM, sig_alrm) == SIG_ERR) {
		return -1;
	}

	if (setjmp(env_alrm) != 0) {
		printf("read timeout!\n");
		return 0;
	}

	alarm(10);
	while ((rd_num = read(STDIN_FILENO, rd_buf, 16)) > 0) {
		write(STDOUT_FILENO, rd_buf, rd_num);
	}

	return 0;
}

static void sig_alrm(int signo)
{
	longjmp(env_alrm, 1);
}

