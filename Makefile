global.o: global.c error-handle.h error-handle.o
	gcc -g -c -Wall -std=c99 global.c
error-handle.o: error-handle.c
	gcc -g -c -Wall -std=c99 error-handle.c

.PHONY:clean
clean:
	rm -f *.o
