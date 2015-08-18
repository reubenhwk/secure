#!/bin/bash

v1=$(cat test/chacha8-test.bin | ./cpt chacha8 | ./cpt md5)

if [ "$v1" != "f7408430cc526210deccbf6a0c67e623" ] ; then
	echo "vector1 failed" >&2
	exit -1
fi

echo success

