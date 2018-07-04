CC=gcc -Wall -O2

OBJS=bin/main.o bin/tty.o bin/buffer.o

pi: $(OBJS)
	$(CC) $^ -o $@

bin/%.o: src/%.c | bin
	$(CC) $^ -c -o $@

bin:
	mkdir bin

.PHONY: clean

clean:
	rm -r bin || true
	rm pi || true
