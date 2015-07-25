
.PHONY: clean

md4: md4.c
	$(CC) -o md4 -DTEST -std=gnu99 -O3 $^

clean:
	rm -f *.o md4

