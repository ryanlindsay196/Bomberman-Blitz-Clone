#include "Engine\Allocators\StackAllocator.h"

void StackAllocator::Init(size_t inBufferSizeInBytes)
{
	buffer = malloc(inBufferSizeInBytes);
	bufferSizeInBytes = inBufferSizeInBytes;
	currentOffset = 0;
}

void * StackAllocator::Alloc(size_t size)
{
	return AllocAlign(size, 2 * sizeof(void*));
}

void StackAllocator::Free(void * ptr)
{
	if (ptr == nullptr)
	{
		return;
	}

	uintptr_t start, end, currAddr;
	StackAllocationHeader* header;
	size_t prevOffset;

	start = (uintptr_t)buffer;

	end = start + (uintptr_t)bufferSizeInBytes;
	currAddr = (uintptr_t)ptr;

	if (!(start <= currAddr && currAddr < end))
	{
		assert(0 && "Out of bounds memory address passed to stack allocator (free)");
		return;
	}

	if (currAddr >= start + (uintptr_t)currentOffset)
	{
		return;
	}

	header = (StackAllocationHeader*)(currAddr - sizeof(StackAllocationHeader));
	prevOffset = (size_t)(currAddr - (uintptr_t)header->padding - start);

	currentOffset = prevOffset;
	this->prevOffset = header->prevOffset;
}

void StackAllocator::Resize(void* ptr, size_t oldSize, size_t newSize)
{
	ResizeAlign(ptr, oldSize, newSize, 2 * sizeof(void*));
}

void StackAllocator::FreeAll()
{
	currentOffset = 0;
}

void * StackAllocator::AllocAlign(size_t size, size_t alignment)
{
	uintptr_t currAddr, nextAddr;
	size_t padding;
	StackAllocationHeader* header;

	assert(IsPowerOfTwo(alignment));

	if (alignment > 128)
	{
		alignment = 128;
	}

	currAddr = (uintptr_t)buffer + currentOffset;
	padding = CalculatePaddingWithHeader(currAddr, (uintptr_t)alignment, sizeof(StackAllocationHeader));
	if (currentOffset + padding + size > bufferSizeInBytes)
	{
		return nullptr;
	}

	prevOffset = currentOffset;
	currentOffset += padding;

	nextAddr = currAddr + (uintptr_t)padding;
	header = (StackAllocationHeader*)(nextAddr - sizeof(StackAllocationHeader));
	header->padding = (uint8_t)padding;

	currentOffset += size;

	return memset((void*)nextAddr, 0, size);
}

void * StackAllocator::ResizeAlign(void * ptr, size_t oldSize, size_t newSize, size_t alignment)
{
	if (ptr == nullptr)
	{
		return AllocAlign(newSize, alignment);
	}
	else if (newSize == 0)
	{
		Free(ptr);
		return nullptr;
	}
	else
	{
		uintptr_t start, end, currAddr;
		size_t minSize = oldSize < newSize ? oldSize : newSize;
		void* newPtr;

		start = (uintptr_t)buffer;
		end = start + (uintptr_t)bufferSizeInBytes;
		currAddr = (uintptr_t)ptr;
		if (!(start <= currAddr && currAddr < end))
		{
			assert(0 && "Out of bounds memory address passed to stack allocator (resize)");
			return nullptr;
		}

		if (currAddr >= start + (uintptr_t)currentOffset)
		{
			return nullptr;
		}

		if (oldSize == newSize)
		{
			return ptr;
		}

		newPtr = AllocAlign(newSize, alignment);
		memmove(newPtr, ptr, minSize);
		return newPtr;
	}
}


