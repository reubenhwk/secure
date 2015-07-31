#!/bin/bash

#define ff(w, x, y, z) (x&y | (~x)&z)
ff () {
	echo "	mov	$1, $2"
	echo "	and	$1, $3"
	echo "	not	$2"
	echo "	and	$2, $4"
	echo "	or	$1, $2"
}

#define gg(w, x, y, z) (x&y | x&z | y&z)
gg () {
	echo "	mov	$1, $2"
	echo "	and	$1, $3"
	echo "	and	$2, $4"
	echo "	and	$3, $4"
	echo "	or	$1, $2"
	echo "	or	$1, $3"
}

#define hh(w, x, y, z) (x ^ y ^ z)
hh () {
	echo "	mov	$1, $2"
	echo "	xor	$1, $3"
	echo "	xor	$1, $4"
}


#define r1(A, B, C, D, i, s) (A = rol(A + ff(B, C, D) + b[i] + 0x00000000, s))
#define r2(A, B, C, D, i, s) (A = rol(A + gg(B, C, D) + b[i] + 0x5A827999, s))
#define r3(A, B, C, D, i, s) (A = rol(A + hh(B, C, D) + b[i] + 0x6ED9EBA1, s))

r1 () {
	ff r8d $2 $3 $4
	echo "	add	$1, r8d"
	echo "	add	$1, [rsi + $5]"
	echo "	rol	$1, $6"
}

r2 () {
	gg r8d $2 $3 $4
	echo "	add	$1, r8d"
	echo "	add	$1, [rsi + $5]"
	echo "	add	$1, 0x5A827999"
	echo "	rol	$1, $6"
}

r3 () {
	hh r8d $2 $3 $4
	echo "	add	$1, r8d"
	echo "	add	$1, [rsi + $5]"
	echo "	add	$1, 0x6ED9EBA1"
	echo "	rol	$1, $6"
}

S11=3
S12=7
S13=11
S14=19
S21=3
S22=5
S23=9
S24=13
S31=3
S32=9
S33=11
S34=15


echo
echo "section	.text"
echo "	global md4_round"
echo
echo "md4_round:"
echo
echo "	;	round 1"
echo "	push	rbx"
r1 eax ebx ecx edx  0 $S11
r1 edx eax ebx ecx  1 $S12
r1 ecx edx eax ebx  2 $S13
r1 ebx ecx edx eax  3 $S14
r1 eax ebx ecx edx  4 $S11
r1 edx eax ebx ecx  5 $S12
r1 ecx edx eax ebx  6 $S13
r1 ebx ecx edx eax  7 $S14
r1 eax ebx ecx edx  8 $S11
r1 edx eax ebx ecx  9 $S12
r1 ecx edx eax ebx 10 $S13
r1 ebx ecx edx eax 11 $S14
r1 eax ebx ecx edx 12 $S11
r1 edx eax ebx ecx 13 $S12
r1 ecx edx eax ebx 14 $S13
r1 ebx ecx edx eax 15 $S14

echo
echo "	;	round 2"
r2 eax ebx ecx edx  0 $S21
r2 edx eax ebx ecx  4 $S22
r2 ecx edx eax ebx  8 $S23
r2 ebx ecx edx eax 12 $S24
r2 eax ebx ecx edx  1 $S21
r2 edx eax ebx ecx  5 $S22
r2 ecx edx eax ebx  9 $S23
r2 ebx ecx edx eax 13 $S24
r2 eax ebx ecx edx  2 $S21
r2 edx eax ebx ecx  6 $S22
r2 ecx edx eax ebx 10 $S23
r2 ebx ecx edx eax 14 $S24
r2 eax ebx ecx edx  3 $S21
r2 edx eax ebx ecx  7 $S22
r2 ecx edx eax ebx 11 $S23
r2 ebx ecx edx eax 15 $S24

echo
echo "	;	round 3"
r3 eax ebx ecx edx  0 $S21
r3 edx eax ebx ecx  8 $S22
r3 ecx edx eax ebx  4 $S23
r3 ebx ecx edx eax 12 $S24
r3 eax ebx ecx edx  2 $S21
r3 edx eax ebx ecx 10 $S22
r3 ecx edx eax ebx  6 $S23
r3 ebx ecx edx eax 14 $S24
r3 eax ebx ecx edx  1 $S21
r3 edx eax ebx ecx  9 $S22
r3 ecx edx eax ebx  5 $S23
r3 ebx ecx edx eax 13 $S24
r3 eax ebx ecx edx  3 $S21
r3 edx eax ebx ecx 11 $S22
r3 ecx edx eax ebx  7 $S23
r3 ebx ecx edx eax 15 $S24
echo "	pop	rbx"
echo
echo "	ret"
echo

