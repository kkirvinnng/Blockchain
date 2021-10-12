#pragma once

// https://csrc.nist.gov/csrc/media/publications/fips/180/2/archive/2002-08-01/documents/fips180-2withchangenotice.pdf

#include <stddef.h>
#include <windows.h>

#define BLOCK_SIZE 32 // SHA256 outputs a 32 byte digest

typedef unsigned int IWORD; // 32-bit word, change to "long" for 16-bit machines

typedef struct {
    BYTE data[64];
    IWORD datalen;
    unsigned long long bitlen;
    IWORD state[8];
} SHA256_Context;

void sha256_init(SHA256_Context *ctx);
void sha256_update(SHA256_Context *ctx, const BYTE data[], size_t len);
void sha256_final(SHA256_Context *ctx, BYTE hash[]);