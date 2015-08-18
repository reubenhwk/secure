#!/bin/bash

cat Makefile.am | ./cpt chacha8 > test/chacha8-test.bin
cat Makefile.am | ./cpt chacha12 > test/chacha12-test.bin
cat Makefile.am | ./cpt chacha20 > test/chacha20-test.bin

HASH=$(md5sum Makefile.am | cut -c 1-32)

for i in 8 12 20 ; do
cat << EOF > test/chacha$i.sh
#!/bin/bash

v1=\$(cat test/chacha$i-test.bin | ./cpt chacha$i | ./cpt md5)

if [ "\$v1" != "$HASH" ] ; then
	echo "vector1 failed" >&2
	exit -1
fi

echo success

EOF
chmod +x test/chacha$i.sh
done

