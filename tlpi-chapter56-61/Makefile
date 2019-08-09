test-order:byte-order.c test-server
	gcc $< -o $@

test-server:server.c test-client
	gcc $< -o $@

test-client:client.c
	gcc $< -o $@

.PHONY:clean
clean:
	rm -f test-* server
