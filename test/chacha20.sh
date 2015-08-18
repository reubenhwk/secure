#!/bin/bash

v1=$(cat test/chacha20-test.bin | ./cpt chacha20 | ./cpt md5)

if [ "$v1" != "6e87d4ecf5d27ff96090f4616ecd3f5c" ] ; then
	echo "vector1 failed" >&2
	exit -1
fi

echo success

