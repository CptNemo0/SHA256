#pragma once
#include <cstdint>
#include <vector>

typedef uint32_t word;
typedef uint64_t word64;

typedef struct
{
	std::vector<word> data;
} block;