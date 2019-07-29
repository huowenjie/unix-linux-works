TYPE :=
TARG :=

ifeq ($(TYPE),ar)
	TARG := libdaemon.a
else
	TARG := libdaemon.so
endif

test-lib:main.c $(TARG)
	gcc $< -o $@ -L. -ldaemon

$(TARG):create-daemon.o
ifeq ($(TYPE),ar)
	ar -r $@ $^
else
	gcc -shared -o $@ $^
endif

create-daemon.o:create-daemon.c create-daemon.h
	gcc -c $^

.PHONY:
clean:
	rm -f test* *.o *.gch *.so *.a
