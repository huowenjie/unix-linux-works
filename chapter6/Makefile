all: longjmp-work env-operate

env-operate: env-operate.c ../error-handle.o ../global.o
	gcc -g $^ -Wall  -o $@
longjmp-work: longjmp-work.c ../error-handle.o ../global.o
	gcc -g $^ -Wall -std=c99 -o $@

.PHONY:clean
clean:
	rm -f *.o longjmp-work env-operate 
