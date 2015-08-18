#!/bin/bash

v1=$(cat test/chacha20-test.bin | ./cpt chacha20 | ./cpt md5)

if [ "$v1" != "77bdfe19163879436d9ec347674d651a" ] ; then
	echo "vector1 failed" >&2
	exit -1
fi

echo success

