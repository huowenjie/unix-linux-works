
test-simple-signal: simple-signal.c ../error-handle.o ../error-handle.h
	gcc -g $^ -o $@

test-sig-receive: signal-receiver.c ../error-handle.o ../error-handle.h
	gcc -g $^ -o $@

.PHONY:
clean:
	rm -f test-simple-signal test-sig-receive
