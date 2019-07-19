#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * 根据 POSIX 需求实现 abort
 * 1.无论阻塞或者忽略SIGABRT abrot 调用不受影响
 * 2.刷新 stdio 流
 * 3.取消屏蔽 SIGABRT 信号
 */
void abort_like()
{
	struct sigaction act;
	sigset_t mask;

	sigaction(SIGABRT, NULL, &act);
	if (act.sa_handler == SIG_IGN) {
		act.sa_handler = SIG_DFL;
		sigaction(SIGABRT, &act, NULL);
	}

	if (act.sa_handler == SIG_DFL) {
		fflush(NULL);
	}

	// 取消屏蔽
	sigfillset(&mask);
	sigdelset(&mask, SIGABRT);

	// 将 mask 指定的信号集，赋给信号掩码
	// mask 中已经剔除 SIGABRT 所以进程不会阻塞 SIGABRT 信号
	sigprocmask(SIG_SETMASK, &mask, NULL);
	kill(getpid(), SIGABRT);

	// 如果进程捕获了 SIGABRT 信号并且并未返回（调用siglongjmp）之类的函数
	// 再次刷新 io 缓冲区发送 SIGABRT 信号并且结束当前进程，确保其被杀死
	fflush(NULL);
	act.sa_handler = SIG_DFL;
	sigaction(SIGABRT, &act, NULL);
	sigprocmask(SIG_SETMASK, &mask, NULL);
	kill(getpid(), SIGABRT);
	exit(1);
}


int main(int argc, char *argv[])
{
	abort_like();
	return 0;
}

