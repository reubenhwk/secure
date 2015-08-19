
#pragma once

#include <stdlib.h>

typedef void (*encrypt_t)(void * ctx, void * _buffer, size_t len);
typedef void (*decrypt_t)(void * ctx, void * _buffer, size_t len);
typedef void (*free_ctx_t)(void * ctx);

typedef struct {
        encrypt_t encrypt;
        encrypt_t decrypt;
        free_ctx_t free;
} crypt_t;

