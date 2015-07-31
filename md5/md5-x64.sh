#!/bin/bash

round () {
	echo "	add	r8d, $1"
	echo "	add	r8d, dword [rsi + $5*4]"
	echo "	add	r8d, $7"
	echo "	rol	r8d, $6"
	echo "	add	r8d, $2"
	echo "	mov	$1, r8d"
}

#define ff(w, x, y, z) (x&y | (~x)&z)
ff () {
	echo "	mov	$1, $2"
	echo "	and	$1, $3"
	echo "	mov	r9d, $2"
	echo "	not	r9d"
	echo "	and	r9d, $4"
	echo "	or	$1, r9d"
}
#define r1(a, b, c, d, k, s, i) (a = b + rol(a + ff(b, c, d) + k + i, s))
r1 () {
	ff r8d $2 $3 $4
	round $@
}


#define gg(w, x, y, z) ((x&z) | (y&(~z)))
gg () {
	echo "	mov	$1, $2"
	echo "	and	$1, $4"
	echo "	mov	r9d, $4"
	echo "	not	r9d"
	echo "	and	r9d, $3"
	echo "	or	$1, r9d"
}
#define r2(a, b, c, d, k, s, i) (a = b + rol(a + gg(b, c, d) + k + i, s))
r2 () {
	gg r8d $2 $3 $4
	round $@
}


#define hh(w, x, y, z) (x ^ y ^ z)
hh () {
	echo "	mov	$1, $2"
	echo "	xor	$1, $3"
	echo "	xor	$1, $4"
}
#define r3(a, b, c, d, k, s, i) (a = b + rol(a + hh(b, c, d) + k + i, s))
r3 () {
	hh r8d $2 $3 $4
	round $@
}

#define ii(w, x, y, z) (y ^ (x | (~z)))
ii () {
	echo "	mov	$1, $4"
	echo "	not	$1"
	echo "	or	$1, $2"
	echo "	xor	$1, $3"
}
#define r4(a, b, c, d, k, s, i) (a = b + rol(a + ii(b, c, d) + k + i, s))
r4 () {
	ii r8d $2 $3 $4
	round $@
}

S11=7
S12=12
S13=17
S14=22
S21=5
S22=9
S23=14
S24=20
S31=4
S32=11
S33=16
S34=23
S41=6
S42=10
S43=15
S44=21


#typedef struct {
#	union {
#		uint32_t _32[4];
#		uint8_t _8[16];
#	} s;
#	union {
#		uint32_t _32[16];
#		uint8_t _8[64];
#	} b;
#	uint64_t count;
#} md_t;


#;  RDI, RSI, RDX, RCX, R8, and R9
echo
echo "section	.text"
echo "	global md5_round"
echo
echo "md5_round:"
echo
echo "	push	rbx"
echo
echo "	mov	eax, dword [rdi + 0]"
echo "	mov	ebx, dword [rdi + 4]"
echo "	mov	ecx, dword [rdi + 8]"
echo "	mov	edx, dword [rdi + 12]"

echo
echo "	;	round 1"
r1 eax ebx ecx edx  0 $S11 0xd76aa478
r1 edx eax ebx ecx  1 $S12 0xe8c7b756
r1 ecx edx eax ebx  2 $S13 0x242070db
r1 ebx ecx edx eax  3 $S14 0xc1bdceee
r1 eax ebx ecx edx  4 $S11 0xf57c0faf
r1 edx eax ebx ecx  5 $S12 0x4787c62a
r1 ecx edx eax ebx  6 $S13 0xa8304613
r1 ebx ecx edx eax  7 $S14 0xfd469501
r1 eax ebx ecx edx  8 $S11 0x698098d8
r1 edx eax ebx ecx  9 $S12 0x8b44f7af
r1 ecx edx eax ebx 10 $S13 0xffff5bb1
r1 ebx ecx edx eax 11 $S14 0x895cd7be
r1 eax ebx ecx edx 12 $S11 0x6b901122
r1 edx eax ebx ecx 13 $S12 0xfd987193
r1 ecx edx eax ebx 14 $S13 0xa679438e
r1 ebx ecx edx eax 15 $S14 0x49b40821

echo
echo "	;	round 2"
r2 eax ebx ecx edx  1 $S21 0xf61e2562
r2 edx eax ebx ecx  6 $S22 0xc040b340
r2 ecx edx eax ebx 11 $S23 0x265e5a51
r2 ebx ecx edx eax  0 $S24 0xe9b6c7aa
r2 eax ebx ecx edx  5 $S21 0xd62f105d
r2 edx eax ebx ecx 10 $S22 0x02441453
r2 ecx edx eax ebx 15 $S23 0xd8a1e681
r2 ebx ecx edx eax  4 $S24 0xe7d3fbc8
r2 eax ebx ecx edx  9 $S21 0x21e1cde6
r2 edx eax ebx ecx 14 $S22 0xc33707d6
r2 ecx edx eax ebx  3 $S23 0xf4d50d87
r2 ebx ecx edx eax  8 $S24 0x455a14ed
r2 eax ebx ecx edx 13 $S21 0xa9e3e905
r2 edx eax ebx ecx  2 $S22 0xfcefa3f8
r2 ecx edx eax ebx  7 $S23 0x676f02d9
r2 ebx ecx edx eax 12 $S24 0x8d2a4c8a

echo
echo "	;	round 3"
r3 eax ebx ecx edx  5 $S31 0xfffa3942
r3 edx eax ebx ecx  8 $S32 0x8771f681
r3 ecx edx eax ebx 11 $S33 0x6d9d6122
r3 ebx ecx edx eax 14 $S34 0xfde5380c
r3 eax ebx ecx edx  1 $S31 0xa4beea44
r3 edx eax ebx ecx  4 $S32 0x4bdecfa9
r3 ecx edx eax ebx  7 $S33 0xf6bb4b60
r3 ebx ecx edx eax 10 $S34 0xbebfbc70
r3 eax ebx ecx edx 13 $S31 0x289b7ec6
r3 edx eax ebx ecx  0 $S32 0xeaa127fa
r3 ecx edx eax ebx  3 $S33 0xd4ef3085
r3 ebx ecx edx eax  6 $S34 0x04881d05
r3 eax ebx ecx edx  9 $S31 0xd9d4d039
r3 edx eax ebx ecx 12 $S32 0xe6db99e5
r3 ecx edx eax ebx 15 $S33 0x1fa27cf8
r3 ebx ecx edx eax  2 $S34 0xc4ac5665

echo
echo "	;	round 4"
r4 eax ebx ecx edx  0 $S41 0xf4292244
r4 edx eax ebx ecx  7 $S42 0x432aff97
r4 ecx edx eax ebx 14 $S43 0xab9423a7
r4 ebx ecx edx eax  5 $S44 0xfc93a039
r4 eax ebx ecx edx 12 $S41 0x655b59c3
r4 edx eax ebx ecx  3 $S42 0x8f0ccc92
r4 ecx edx eax ebx 10 $S43 0xffeff47d
r4 ebx ecx edx eax  1 $S44 0x85845dd1
r4 eax ebx ecx edx  8 $S41 0x6fa87e4f
r4 edx eax ebx ecx 15 $S42 0xfe2ce6e0
r4 ecx edx eax ebx  6 $S43 0xa3014314
r4 ebx ecx edx eax 13 $S44 0x4e0811a1
r4 eax ebx ecx edx  4 $S41 0xf7537e82
r4 edx eax ebx ecx 11 $S42 0xbd3af235
r4 ecx edx eax ebx  2 $S43 0x2ad7d2bb
r4 ebx ecx edx eax  9 $S44 0xeb86d391

echo
echo "	add	dword [rdi + 0], eax"
echo "	add	dword [rdi + 4], ebx"
echo "	add	dword [rdi + 8], ecx"
echo "	add	dword [rdi + 12], edx"

echo
echo "	pop	rbx"
echo
echo "	ret"
echo

