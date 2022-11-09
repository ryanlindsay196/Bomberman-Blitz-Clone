#pragma once

#include "Engine/Allocators/AllocatorCommon.h"

size_t CalculatePaddingWithHeader(uintptr_t ptr, uintptr_t alignment, size_t headerSize)
{
	uintptr_t p, a, modulo, padding, neededSpace;

	assert(IsPowerOfTwo(alignment));

	p = ptr;
	a = alignment;
	modulo = p & (a - 1);

	padding = 0;
	neededSpace = 0;

	if (modulo != 0)
	{
		padding = a - modulo;
	}

	neededSpace = (uintptr_t)headerSize;

	if (padding < neededSpace)
	{
		neededSpace -= padding;

		if ((neededSpace & (a - 1)) != 0)
		{
			padding += a * (1 + (neededSpace / a));
		}
		else
		{
			padding += a * (neededSpace / a);
		}
	}

	return (size_t)padding;
}
