
#pragma once

#include <stdint.h>
#include <stdlib.h>

#define MD_LENGTH 16

typedef struct {
	union {
		uint32_t _32[4];
		uint8_t _8[16];
	} s;
	union {
		uint32_t _32[16];
		uint8_t _8[64];
	} b;
	uint64_t count;
} md_t;

typedef void (*MD_ROUNDS_PROC)(md_t*, uint32_t const*);

md_t * MD_Init(md_t * md);
void MD_Update(md_t * md, unsigned char const * d, size_t len, MD_ROUNDS_PROC);
void MD_Final(md_t * md, unsigned char * digest, size_t len, MD_ROUNDS_PROC);

