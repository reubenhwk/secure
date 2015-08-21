
#pragma once

#include <stdint.h>

typedef struct {
	uint8_t values[32];
} curve25519_value_t;

typedef curve25519_value_t curve25519_key_t;

int curve25519_donna(curve25519_key_t *mypublic, curve25519_key_t *secret, curve25519_value_t const *basepoint);

