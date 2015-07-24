
.PHONY: clean

md4: main.o md4.o
	$(CC) $^ -o $@

%.o: %.c
	$(CC) -c $< -std=gnu99 -g -O3

clean:
	rm -f *.o md4

