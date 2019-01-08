all: fcntl-dup atomic-append append-work write-test readv-writev

readv-writev: readv-writev.c ../error-handle.o ../global.o
	gcc -g $^ -Wall -std=c99 -o $@

.PHONY:clean
clean:
	rm -f *.o  
