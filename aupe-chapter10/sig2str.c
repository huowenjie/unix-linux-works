#include <signal.h>
#include <string.h>
#include <stdio.h>

// psignal(int, const char *)
// psiginfo(const siginfo_t *, cont char *)
// strsignal(int)

int main(int argc, char *argv[])
{
	const char *str = NULL;
	int i = 0;
	int count = (int)(sizeof(_sys_siglist) / sizeof(char *));

	for (; i < count; i++) {
		printf("----------->%s\n", _sys_siglist[i]);
	}

	return 0;
}

