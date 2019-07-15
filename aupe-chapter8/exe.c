#include <sys/times.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	struct  tms start = { 0 };
	struct  tms end   = { 0 };
	clock_t clc_start = 0;
	clock_t clc_end   = 0;
	clock_t clc_tck   = 0;

	if (argc != 2) {
		return 0;
	}

	if ((clc_start = times(&start)) < 0) {
		printf("start time error!\n");
	}

	if (system(argv[1]) < 0) {
		printf("system date error!\n");
	}

	if ((clc_end = times(&end)) < 0) {
		printf("end time error!\n");
	}

	if ((clc_tck = sysconf(_SC_CLK_TCK)) <= 0) {
		printf("get tck error!\n");
		exit(-1);
	}

	printf("real time = %lf\n", (clc_end - clc_start) / (double)clc_tck);
	printf("user cpu  = %lf\n", (end.tms_utime - start.tms_utime) / (double)clc_tck);
	printf("sys  cpu  = %lf\n", (end.tms_stime - start.tms_stime) / (double)clc_tck);
	printf("user child= %lf\n", (end.tms_cutime - start.tms_cutime) / (double)clc_tck);
	printf("sys  child= %lf\n", (end.tms_cstime - start.tms_cstime) / (double)clc_tck);

	printf("exe uid = %d, euid = %d\n", getuid(), geteuid());
	return 0;
}

