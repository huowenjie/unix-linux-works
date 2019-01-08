#include <stdio.h>
#include <setjmp.h>
#include "../error-handle.h"
#include "../global.h"

static jmp_buf env;

int test(int ret)
{
	printf("ret = %d\n", ret);

	return 0;
}

int main()
{
	int ret = setjmp(env);
	test(ret);

	if (ret < 1) {
		longjmp(env, 1);
	}

	return 0;
}

