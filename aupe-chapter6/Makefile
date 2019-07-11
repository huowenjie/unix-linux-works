DEP_PATH := ..

uname-info:uname-info.o $(DEP_PATH)/error-handle.o
	gcc $^ -o $@
uname-info.o:uname-info.c $(DEP_PATH)/error-handle.h
	gcc -c $<

.PHONT:
clean:
	rm -f *.o uname-info
