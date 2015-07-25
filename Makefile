
.PHONY: clean

TARGETS=cpt-md4 cpt-md5

default: $(TARGETS)

cpt-md4: md4/md4.c
	$(CC) -o $@ -DTEST -std=gnu99 -O3 $^

cpt-md5: md5/md5.c
	$(CC) -o $@ -DTEST -std=gnu99 -O3 $^

clean:
	rm -f *.o cpt-md4 cpt-md5

