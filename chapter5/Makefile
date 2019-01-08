all: fcntl-dup atomic-append append-work write-test readv-writev

readv-writev: readv-writev.c ../error-handle.o ../global.o
	gcc -g $^ -Wall -std=c99 -o $@
write-test: write-test.c ../error-handle.o ../global.o
	gcc -g $^ -Wall -std=c99 -o $@
fcntl-dup: fcntl-dup.c ../error-handle.o ../global.o
	gcc -g $^ -Wall -std=c99 -o $@
atomic-append: atomic-append.o ../error-handle.o ../global.o
	gcc -g $^ -Wall -std=c99 -o $@
append-work: append-work.c ../error-handle.o ../global.o
	gcc -g $^ -Wall -std=c99 -o $@

.PHONY:clean
clean:
	rm -f *.o append-work atomic-append fcntl-dup write-test readv-writev
