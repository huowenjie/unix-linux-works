SOURCE  := $(wildcard *.c)
TARGETS := $(patsubst %.c, %, $(SOURCE))

all:$(TARGETS)

$(TARGETS):%:%.c
	gcc $< -o $@ -pthread

.PHONY:clean
clean:
	rm -f ${TARGETS}

