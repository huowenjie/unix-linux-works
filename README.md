2018-12-26
	chapter4-1 实现一个 tee 命令，可以解析 -a 参数 -- huowj

2019-01-01
	新年快乐！
	chapter4-1 在本地更新仓库代码，完成 chapter4-1 tee 题目 -- huowj from home

2019-01-02
	chapter4-1 完成一些工具函数 -- huowj from company

2019-01-04
	global.h/global.c 定义了保存选项和值的结构，在启动应用程序时：
	1.调用 option_new 来生成参数列表；
	2.调用 option_iterator 遍历参数列表；
	3.调用 option_item 获取指定参数的选项和值；
	4.调用 option_get_opt/option_get_val 获取指定位置对应参数的选项和值；
	5.在结束程序时，需要调用 option_free 释放参数列表。

	对于 3、4 选项功能有所重复，对于 3 则是传入函数指针，通过回调函数的方式
	获取信息；对于 4 则是直接获取信息。
								-- huowj from company

