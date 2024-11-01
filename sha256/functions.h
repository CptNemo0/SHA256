#pragma once

#include <cstdint>
#include <limits.h>
#include "typedef.h"

static inline word RotR(word A, uint32_t n)
{
	return (A >> n) | (A << (32 - n));
}

static inline word ShR(word A, uint32_t n)
{
	return (A >> n);
}

static inline word64 Concatenate(word A, word B)
{
	word64 a = A;
	word64 b = B;
	return ((a << 32) | b);
}

static inline word Ch(word X, word Y, word Z)
{
	return (X & Y) ^ (~X & Z);
}

static inline word Maj(word X, word Y, word Z)
{
	return (X & Y) ^ (X & Z) ^ (Y & Z);
}

static inline word Sum0(word X)
{
	return RotR(X, 2) ^ RotR(X, 13) ^ RotR(X, 22);
}

static inline word Sum1(word X)
{
	return RotR(X, 6) ^ RotR(X, 11) ^ RotR(X, 25);
}

static inline word Sigma0(word X)
{
	return RotR(X, 7) ^ RotR(X, 18) ^ ShR(X, 3);
}

static inline word Sigma1(word X)
{
	return RotR(X, 17) ^ RotR(X, 19) ^ ShR(X, 10);
}