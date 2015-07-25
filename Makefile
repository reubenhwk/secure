
.PHONY: clean

cpt-md4: md4/md4.c
	$(CC) -o $@ -DTEST -std=gnu99 -O3 $^

clean:
	rm -f *.o cpt-md4

