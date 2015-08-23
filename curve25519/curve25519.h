
#pragma once

#include <stdint.h>

#define CURVE22519_COUNT 32
typedef struct {
	uint8_t values[CURVE22519_COUNT];
} curve25519_value_t;

typedef curve25519_value_t curve25519_key_t;

curve25519_key_t curve25519_private(void);
curve25519_key_t curve25519_public(curve25519_key_t const * private_key);
void curve25519(
	curve25519_key_t * mypublic,
	curve25519_key_t const * secret,
	curve25519_value_t const *basepoint);
