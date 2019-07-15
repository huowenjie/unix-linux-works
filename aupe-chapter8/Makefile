create-process:create-process.c double-fork
	gcc $< -o $@

double-fork:double-fork.c exe
	gcc $< -o $@

exe:exe.c printuid
	gcc $< -o $@

printuid:printuid.c
	gcc $< -o $@

.PHONY:
clean:
	rm -f *.o create-process double-fork exe printuid
