#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include "error-handle.h"

void err_exit(const char *format, ...)
{
	va_list args;

	va_start(args, format);
	fprintf(stderr, format, args);
	va_end(args);

	exit(-1);
}

void err_code_exit(int err_code)
{
	char *err_str = strerror(err_code);
	if (err_str) {
		perror(err_str);
	}

	exit(err_code);
}

void err_fatal(const char *format, ...)
{
	va_list args;

	va_start(args, format);
	fprintf(stderr, format, args);
	va_end(args);

	exit(-1);	
}

void err_not_exit(const char *format, ...)
{
	va_list args;

	va_start(args, format);
	fprintf(stderr, format, args);
	va_end(args);
}

void err_code_not_exit(int err_code)
{
	char *err_str = strerror(err_code);
	if (err_str) {
		perror(err_str);
	}
}

