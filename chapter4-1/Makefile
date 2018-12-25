
tee-work: tee-work.o ../error-handle.o
	gcc $^ -Wall -std=c99 -o $@
tee-work.o: tee-work.c ../error-handle.h
	gcc -Wall -std=c99 -c $<

.PHONY:clean
clean:
	rm -f *.o tee-work
