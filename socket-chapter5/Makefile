test-multiple:multiple-clent.c test-client-ex
	gcc $< -o $@

test-client-ex:client-ex.c test-server-ex
	gcc $< -o $@

test-server-ex:server-ex.c test-client
	gcc $< -o $@

test-client:time-client.c test-server
	gcc $< -o $@

test-server:time-server.c test-order
	gcc $< -o $@

test-order:byte-order.c
	gcc $< -o $@

.PHONY:clean
clean:
	rm -f test-*
