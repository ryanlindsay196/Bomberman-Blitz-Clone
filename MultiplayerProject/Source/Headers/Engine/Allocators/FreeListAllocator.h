#pragma once

#include "Engine/Allocators/AllocatorCommon.h"

#include <stdlib.h>

typedef struct FreeListAllocationHeader FreeListAllocationHeader;
struct FreeListAllocationHeader
{
	size_t blockSize;
	size_t padding;
};

typedef struct FreeListNode FreeListNode;
struct FreeListNode 
{
	FreeListNode* next;
	size_t blockSize;
};

//typedef PlacementPolicy PlacementPolicy;
enum class PlacementPolicy
{
	FindFirst,
	FindBest
};

class FreeListAllocator
{
public:
	void FreeAll();

	void Init(size_t inBufferSizeInBytes);

	void* Alloc(size_t size, size_t alignment);

	void Free(void* ptr);

	void Destroy();

private:
	FreeListNode* FindFirst(size_t size, size_t alignment, size_t* padding_, FreeListNode** prevNode_);

	FreeListNode* FindBest(size_t size, size_t alignment, size_t* padding_, FreeListNode** prevNode_);

	void Coalescence(FreeListNode*prevNode, FreeListNode* freeNode);

	void NodeInsert(FreeListNode** phead, FreeListNode* prevNode, FreeListNode* newNode);

	void NodeRemove(FreeListNode** phead, FreeListNode* prevNode, FreeListNode* delNode);

	void* data;
	size_t size;
	size_t used;

	FreeListNode* head;
	PlacementPolicy policy;
};