
.PHONY: clean

md4: main.o md4.o

%.o: %.c
	$(CC) -c $< -std=gnu99

clean:
	rm -f *.o md4

