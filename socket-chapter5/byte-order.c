#include <stdio.h>

int main(int argc, char *argv[])
{
	int i = 0;

	union data {
		short sh;
		char ch[sizeof(short)];
	} dt;

	dt.sh = 0x0102;

	for (i = 0; i < sizeof(short); i++) {
		printf("ch[%d] = 0x%02x\n", i, dt.ch[i]);
	}

	return 0;
}
