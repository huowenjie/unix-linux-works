SUBDIRS := aupe-chapter3 aupe-chapter4

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
