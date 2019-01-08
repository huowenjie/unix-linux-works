#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../error-handle.h"
#include "../global.h"

static char env_str[255] = { 0 };

int setenv_self(const char *name, const char *value, int overwrite)
{
	char  *buff = NULL;
	size_t buff_len = 0;
	int	   ret = SUCCESS;

	if (!name || !value || !name[0]) {
		err_code_not_exit(EINVAL);
		return FAILED;
	}

	if (!overwrite) {
		return SUCCESS;
	}

	buff_len = strlen(name) + strlen(value) + 2;
	buff = (char *)malloc(buff_len);
	if (!buff) {
		err_exit("memory not enough!\n");
	}

	memset(buff, 0, buff_len);
	strcpy(env_str, name);
	strcat(env_str, "=");
	strcat(env_str, value);

	ret = putenv(env_str);
	free(buff);

	return ret;
}

int unsetenv_self(const char *name)
{
	int 	count = 0;
	int 	ret   = SUCCESS;
	char 	*buff = NULL;
	size_t  buff_len = 0;

	if (!name || !name[0]) {
		err_code_not_exit(EINVAL);
		return FAILED;
	}

	buff_len = strlen(name) + 2;
	buff = (char *)malloc(buff_len);

	if (!buff) {
		err_exit("memory not enough!\n");
	}

	memset(buff, 0, buff_len);
	strcpy(buff, name);
	strcat(buff, "=");

	while (getenv(name) && count++ < 100) {
		if ((ret = putenv(buff)) != SUCCESS) {
			break;
		}
	}

	free(buff);

	return ret;
}

int main(int argc, char *argv[])
{
	int ret   = 0;

	if (argc == 3) {
		ret = setenv_self(argv[1], argv[2], 1);
		printf("name = %s\nvalue = %s\n", argv[1], getenv(argv[1]));
	} else if (argc == 2) {
		ret = unsetenv_self(argv[1]);
	}

	printf("ret = %d\n", ret);

	return 0;
}

