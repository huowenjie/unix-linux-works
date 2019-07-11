info:info.c access
	gcc -g $< -o $@

access:access.c chmodexe
	gcc -g $< -o $@

chmodexe:chmod.c cavity
	gcc -g $< -o $@

cavity:cavity.c copy
	gcc -g $< -o $@

copy:copy.c filetree
	gcc -g $< -o $@

filetree:filetree.c
	gcc -g $< -o $@

.PHONY:
clean: 
	rm -f info access chmodexe cavity copy filetree *.txt
