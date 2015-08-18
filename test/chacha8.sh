#!/bin/bash

v1=$(cat test/chacha8-test.bin | ./cpt chacha8 | ./cpt md5)

if [ "$v1" != "2b18d3f6917ab6030eec668fa029b596" ] ; then
	echo "vector1 failed" >&2
	exit -1
fi

echo success

