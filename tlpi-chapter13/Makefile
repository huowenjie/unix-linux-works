test-copy:copy.c test-chmod
	gcc $< -o $@ -DBUF_SIZE=4096

test-chmod:chmod-a-x.c ../error-handle.o ../error-handle.h
	gcc $^ -o $@

.PHONY:
clean:
	rm -f test-*
