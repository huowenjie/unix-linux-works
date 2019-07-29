SUBDIRS := aupe-chapter3 aupe-chapter4 aupe-chapter6 \
		   aupe-chapter8 aupe-chapter9 tlpi-chapter12 \
		   tlpi-chapter13 \
		   tlpi-chapter20 \
		   tlpi-chapter21 \
		   tlpi-chapter24-26 \
		   tlpi-chapter29-33 \
		   tlpi-chapter34-36 \
		   tlpi-chapter37-38 \
		   tlpi-chapter41-42

global.o: global.c error-handle.h error-handle.o
	gcc -g -c -Wall -std=c99 global.c
error-handle.o: error-handle.c
	gcc -g -c -Wall -std=c99 error-handle.c

.PHONY: subdirs $(SUBDIRS)
subdirs: $(SUBDIRS)
$(SUBDIRS):
	make -C $@

.PHONY: subclean
subclean:
	for dir in $(SUBDIRS); do \
		make -C $$dir clean; \
	done

.PHONY: clean
clean:
	rm -f *.o
