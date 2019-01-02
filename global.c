#include <stdlib.h>
#include "global.h"
#include "error-handle.h"

struct OPTION_st
{
	char *opt;
	char *val;
};

/*
 * 首先遍历参数，遇到 ‘-’ 开头的字符串则将之定义为 “选项” 保存，同时
 * 根据 next 变量判断是否是下一个参数选项和值，最后一个元素赋空。
 */
OPTION * option_new(int num, char *argv[])
{
	OPTION *opts 	= NULL;
	char   *opt_str = NULL;
	int		next	= 1;
	int     i 		= 0;
	int		j 		= 1;

	if (num <= 1) {
		return NULL;
	}

	opts = (OPTION *)malloc(num * sizeof(OPTION));
	if (!opts) {
		err_exit("memory not enough!\n");
	}

	while (i < num - 1 && j < num) {
		opt_str = argv[j++];

		if (opt_str[0] == '-') {
			i += (!next);
			opts[i].opt = opt_str + 1;
			opts[i].val = NULL;
			
			/* 
			 * next 代表是否进行下一项赋值，上一项包含 ‘-’，指针
			 * 不变且赋值后后移，如果是连续选项，指针直接后移。
			 */
			next = 0;
		} else {
			if (!next) {
				next = !next;
			} else {
				opts[i].opt = NULL;
			}

			opts[i++].val = opt_str;
		}
	}

	opts[num - 1].opt = NULL;
	opts[num - 1].val = NULL;

	return opts;
}

int option_iterator(
	const OPTION *opt, void (*func)(const char *str_opt, const char *str_val))
{
	if (!opt) {
		return FAILED;
	}

	if (!func) {
		return FAILED;
	}

	const OPTION *opt_list = opt;
	while (opt_list->opt || opt_list->val) {
		func(opt_list->opt, opt_list->val);
		opt_list++;
	}

	return SUCCESS;
}

int option_item(
	const OPTION *opt, int index, void (*func)(const char *str_opt, const char *str_val))
{
	int i = 0;

	if (!opt) {
		return FAILED;
	}

	if (!func) {
		return FAILED;
	}

	const OPTION *opt_list = opt;
	while (i++ <= index && (opt_list->opt || opt_list->val)) {
		opt_list++;
	}

	func(opt_list->opt, opt_list->val);

	return SUCCESS;
}

char * option_get_opt(const OPTION *opt, int index)
{
	int i = 0;

	if (!opt) {
		err_exit("null pointer!\n");
	}

	const OPTION *opt_list = opt;
	while (i++ <= index && (opt_list->opt || opt_list->val)) {
		opt_list++;
	}

	return opt_list->opt;
}

char * option_get_val(const OPTION *opt, int index)
{
	int i = 0;

	if (!opt) {
		err_exit("null pointer!\n");
	}

	const OPTION *opt_list = opt;
	while (i++ <= index && (opt_list->opt || opt_list->val)) {
		opt_list++;
	}

	return opt_list->val;
}

void option_free(OPTION *opt)
{
	if (opt) {
		free(opt);
	}
}

