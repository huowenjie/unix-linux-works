test-createthread:create-thread.c test-join-self
	gcc $< -o $@ -pthread

test-join-self:join-self.c test-tree
	gcc $< -o $@ -pthread

test-tree: use-tree.c binary-tree.o binary-tree.h
	gcc -g $^ -o $@ -pthread

binary-tree.o:binary-tree.c
	gcc -g -c $<

.PHONY:
clean:
	rm -f test-* *.o
