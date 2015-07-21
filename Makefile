
.PHONY: clean

md4: main.o md4.o

%.o: %.c
	$(CC) -c $< -std=gnu99 -g

clean:
	rm -f *.o md4

