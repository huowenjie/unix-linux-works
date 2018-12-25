error-handle.o: error-handle.c
	gcc -c -Wall -std=c99 error-handle.c

.PHONY:clean
clean:
	rm -f *.o
