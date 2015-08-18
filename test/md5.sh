#!/bin/bash

v1=$(echo -n | ./cpt md5)
v2=$(echo -n a | ./cpt md5)
v3=$(echo -n abc | ./cpt md5)
v4=$(echo -n "message digest" | ./cpt md5)
v5=$(echo -n "abcdefghijklmnopqrstuvwxyz" | ./cpt md5)
v6=$(echo -n "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789" | ./cpt md5)
v7=$(echo -n "12345678901234567890123456789012345678901234567890123456789012345678901234567890" | ./cpt md5)

if [ "$v1" != "d41d8cd98f00b204e9800998ecf8427e" ] ; then
	echo "vector1 failed" >&2
	exit -1
fi

if [ "$v2" != "0cc175b9c0f1b6a831c399e269772661" ] ; then
	echo "vector2 failed" >&2
	exit -1
fi

if [ "$v3" != "900150983cd24fb0d6963f7d28e17f72" ] ; then
	echo "vector3 failed" >&2
	exit -1
fi

if [ "$v4" != "f96b697d7cb7938d525a2f31aaf161d0" ] ; then
	echo "vector4 failed" >&2
	exit -1
fi

if [ "$v5" != "c3fcd3d76192e4007dfb496cca67e13b" ] ; then
	echo "vector5 failed" >&2
	exit -1
fi

if [ "$v6" != "d174ab98d277d9f5a5611c2c9f419d9f" ] ; then
	echo "vector6 failed" >&2
	exit -1
fi

if [ "$v7" != "57edf4a22be3c955ac49da2e2107b67a" ] ; then
	echo "vector7 failed" >&2
	exit -1
fi

