#include <stdio.h>

int main(int argc, char *argv[])
{
	unsigned int num = 0x12345678;
	char *bt = (char *)(&num);

	printf("origin number = 0x%08x\n", num);
	printf("byte[0] = 0x%02x\n", bt[0]);
	printf("byte[1] = 0x%02x\n", bt[1]);
	printf("byte[2] = 0x%02x\n", bt[2]);
	printf("byte[3] = 0x%02x\n", bt[3]);

	return 0;
}
