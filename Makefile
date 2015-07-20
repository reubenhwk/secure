
.PHONY: clean

md4.o: md4.c

.o.c:
	$(CC) -c $<

clean:
	rm -f *.o a.out

