test-create: create-proc.c test-printid
	gcc $< -o $@

test-printid: printid.c test-zombie
	gcc $< -o $@

test-zombie:create-zombie.c test-three
	gcc $< -o $@

test-three:three-proc.c
	gcc $< -o $@

.PHONY:
clean:
	rm -f test-*
