
append-work: append-work.o ../error-handle.o ../global.o
	gcc -g $^ -Wall -std=c99 -o $@
append-work.o: append-work.c ../error-handle.h ../global.h
	gcc -g -Wall -std=c99 -c $<

.PHONY:clean
clean:
	rm -f *.o append-work
