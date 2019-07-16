test-alarm:alarm.c test-time-limit
	gcc $< -o $@

test-time-limit:time-limit.c test-signal
	gcc $< -o $@

test-signal:signal-test.c test-sig2str
	gcc $< -o $@

test-sig2str:sig2str.c
	gcc $< -o $@

.PHONY:
clean:
	rm -f test-*
