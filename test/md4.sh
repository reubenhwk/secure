#!/bin/bash

v1=$(echo -n | ./cpt md4)
v2=$(echo -n a | ./cpt md4)
v3=$(echo -n abc | ./cpt md4)
v4=$(echo -n "message digest" | ./cpt md4)
v5=$(echo -n "abcdefghijklmnopqrstuvwxyz" | ./cpt md4)
v6=$(echo -n "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789" | ./cpt md4)
v7=$(echo -n "12345678901234567890123456789012345678901234567890123456789012345678901234567890" | ./cpt md4)

if [ "$v1" != "31d6cfe0d16ae931b73c59d7e0c089c0" ] ; then
	echo "vector1 failed" >&2
	exit -1
fi

if [ "$v2" != "bde52cb31de33e46245e05fbdbd6fb24" ] ; then
	echo "vector2 failed" >&2
	exit -1
fi

if [ "$v3" != "a448017aaf21d8525fc10ae87aa6729d" ] ; then
	echo "vector3 failed" >&2
	exit -1
fi

if [ "$v4" != "d9130a8164549fe818874806e1c7014b" ] ; then
	echo "vector4 failed" >&2
	exit -1
fi

if [ "$v5" != "d79e1c308aa5bbcdeea8ed63df412da9" ] ; then
	echo "vector5 failed" >&2
	exit -1
fi

if [ "$v6" != "043f8582f241db351ce627e153e7f0e4" ] ; then
	echo "vector6 failed" >&2
	exit -1
fi

if [ "$v7" != "e33b4ddc9c38f2199c3e7b164fcc0536" ] ; then
	echo "vector7 failed" >&2
	exit -1
fi

echo success

