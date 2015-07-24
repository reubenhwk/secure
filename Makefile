
.PHONY: clean

md4: main.o md4.o

%.o: %.c
	$(CC) -c $< -std=gnu99 -g -O3 -fomit-frame-pointer

clean:
	rm -f *.o md4

