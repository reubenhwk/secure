
.PHONY: clean

md4: main.o md4.o

.o.c:
	$(CC) -c $<

clean:
	rm -f *.o md4

