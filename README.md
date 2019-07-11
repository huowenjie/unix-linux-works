2019-07-11
	调整目录结构，跟进 AUPE 课后习题。
	挑选编程习题，开放式完成。

2019-01-08
	将每一章节的答案放置于 chapterx 下，不再为练习题单独创建目录，在总目录下
	输入命令 make subdir 可以编译子目录下的代码，输入 make subclean 可以清除
	子目录下的编译文件，子目录下的由程序运行而生成的文件需要用户手动清理。

	完成了 chapter5 的练习，部分题目的完成度与题目本身要求有些许不同。

	对于第六章的重点：
	1.进程是一个可执行程序的实例；
	2.多个进程运行的可以是同一个程序；
	3.进程号和父进程号的概念，如何获取进程号；
	4.进程内存布局的了解，堆和栈的区别，各种变量的初始化顺序要鼓捣清楚；
	5.了解什么叫段（segment），了解静态分配内存的特点；
	6.了解虚拟内存管理的原理，以及它的优势；
	7.掌握命令行参数（argc， argv）的用法；
	8.掌握环境列表和环境变量的用法-如何读取、设置环境变量，如何从程序中直接访问
	  环境变量；
	9.了解非局部跳转 API 的用法；
	10.完成课后练习题。

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

2019-01-02
	chapter4-1 完成一些工具函数 -- huowj from company

2019-01-01
	新年快乐！
	chapter4-1 在本地更新仓库代码，完成 chapter4-1 tee 题目 -- huowj from home

2018-12-26
	chapter4-1 实现一个 tee 命令，可以解析 -a 参数 -- huowj

