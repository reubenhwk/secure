
#pragma once

#include "md/md.h"

typedef md_t md5_t;

#define MD5_Init MD_Init
void MD5_Update(md5_t * md, unsigned char const * d, size_t len);
void MD5_Final(md5_t * md, unsigned char * digest, size_t len);

