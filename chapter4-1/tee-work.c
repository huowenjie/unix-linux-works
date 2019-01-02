#include <stdio.h>
#include "../error-handle.h"
#include "../global.h"

void show(const char *str_opt, const char *str_val)
{
	printf("opt = %s val = %s\n", str_opt, str_val);
}

int main(int argc, char *argv[])
{
	OPTION *opt = option_new(argc, argv);

	option_iterator(opt, show);

	option_free(opt);

	return 0;
}

