#pragma once
#include <cstdint>

#include <assert.h>
#include <stdlib.h>
#include <vcruntime_string.h>

inline bool IsPowerOfTwo(uintptr_t x)
{
	return (x & (x - 1)) == 0;
}

size_t CalculatePaddingWithHeader(uintptr_t ptr, uintptr_t alignment, size_t headerSize);