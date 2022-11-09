#pragma once

#include "AllocatorCommon.h"


typedef struct StackAllocationHeader StackAllocationHeader;
struct StackAllocationHeader
{
	size_t prevOffset;
	uint8_t padding;
};

class StackAllocator
{
public:
	void Init(size_t inBufferSize);
	void* Alloc(size_t size);

	void Free(void* ptr);

	void Resize(void* ptr, size_t oldSize, size_t newSize);
	void FreeAll();

private:
	void* AllocAlign(size_t size, size_t alignment);
	void* ResizeAlign(void* ptr, size_t oldSize, size_t newSize, size_t alignment);

	void* buffer;
	size_t bufferSizeInBytes;
	size_t prevOffset;
	size_t currentOffset;
};