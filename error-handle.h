#ifndef ERROR_HANDLE_H
#define ERROR_HANDLE_H

#include <errno.h>

/*
 * 打印错误并终止程序
 */
void err_exit(const char *format, ...);

/*
 * 根据错误码打印错误并终止程序
 */
void err_code_exit(int err_code);

/*
 * 致命错误
 */
void err_fatal(const char *format, ...);

/*
 * 常规错误, 但不终止程序
 */
void err_not_exit(const char *format, ...);
void err_code_not_exit(int err_code);

#endif // ERROR_HANDLE_H

