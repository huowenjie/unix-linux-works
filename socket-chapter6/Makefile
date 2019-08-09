test-svr-select:server-select.c test-client
	gcc -g $< -o $@

test-client:client-io.c test-server
	gcc -g $< -o $@

test-server:server-io.c
	gcc -g $< -o $@

.PHONY: clean
clean:
	rm -f test-*
