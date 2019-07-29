#process group
test-getpgpid: getpgid.c test-creats
	gcc $< -o $@

test-creats:create-session.c
	gcc $< -o $@

.PHONY:
clean:
	rm -f test-*

