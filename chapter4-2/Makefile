
cp-work: cp-work.o ../error-handle.o ../global.o
	gcc -g $^ -Wall -std=c99 -o $@
cp-work.o: cp-work.c ../error-handle.h ../global.h
	gcc -g -Wall -std=c99 -c $<

.PHONY:clean
clean:
	rm -f *.o cp-work
