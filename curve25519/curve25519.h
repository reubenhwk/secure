
#pragma once

#include <stdint.h>

#define CURVE22519_COUNT 32

typedef struct {
	uint8_t values[CURVE22519_COUNT];
} curve25519_value_t;

typedef struct {
	uint8_t values[CURVE22519_COUNT];
} curve25519_key_t;

curve25519_key_t curve25519_generate_private(void);
curve25519_key_t curve25519_compute_public(curve25519_key_t const * private_key);

void curve25519_compute_secret(
	curve25519_value_t * secret,
	curve25519_key_t const * private_key,
	curve25519_key_t const * public_key);
