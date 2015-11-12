
#pragma once

#include "md/md.h"

typedef md_t md4_t;

#define MD4_LENGTH MD_LENGTH
#define MD4_Init MD_Init
void MD4_Update(md4_t * md, unsigned char const * d, size_t len);
void MD4_Final(md4_t * md, unsigned char * digest, size_t len);

