test-cli-udp:client-udp.c test-svr-udp
	gcc -g $< -o $@

test-svr-udp:server-udp.c
	gcc -g $< -o $@

.PHONY: clean
clean:
	rm -f test-*
