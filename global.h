#ifndef WORK_GLOBAL_H
#define WORK_GLOBAL_H

#define SUCCESS  0	// 成功
#define FAILED  -1 	// 通用失败

/*
 * 定义保存选项和值的结构
 */
typedef struct OPTION_st OPTION;

/*
 * 为保存选项的数组申请内存
 */
OPTION * option_new(int num, char *argv[]);

/*
 * 遍历数组，并将选项和值传入回调函数
 */
int option_iterator(
	const OPTION *opt, void (*func)(const char *str_opt, const char *str_val));

/*
 * 释放数组
 */
void option_free(OPTION *opt);

#endif // WORK_GLOBAL_H

