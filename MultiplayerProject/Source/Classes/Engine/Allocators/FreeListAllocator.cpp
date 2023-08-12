#include "Engine\Allocators\FreeListAllocator.h"

void FreeListAllocator::FreeAll()
{
	used = 0;
	FreeListNode* firstNode = (FreeListNode*)data;
	firstNode->blockSize = size;
	firstNode->next = nullptr;
	head = firstNode;
}

void FreeListAllocator::Init(size_t inBufferSizeInBytes)
{
	data = malloc(inBufferSizeInBytes);
	size = inBufferSizeInBytes;
	FreeAll();
}

FreeListNode * FreeListAllocator::FindFirst(size_t size, size_t alignment, size_t * padding_, FreeListNode ** prevNode_)
{
	FreeListNode* node = head;
	FreeListNode* prevNode = nullptr;

	size_t padding = 0;

	while (node != nullptr)
	{
		padding = CalculatePaddingWithHeader((uintptr_t)node, (uintptr_t)alignment, sizeof(FreeListAllocationHeader));
		size_t requiredSpace = size + padding;
		if (node->blockSize >= requiredSpace)
		{
			break;
		}
		prevNode = node;
		node = node->next;
	}

	if (padding_) *padding_ = padding;
	if (prevNode_) *prevNode_ = prevNode;
	return node;
}

FreeListNode * FreeListAllocator::FindBest(size_t size, size_t alignment, size_t * padding_, FreeListNode ** prevNode_)
{
	size_t smallestDiff = ~(size_t)0;

	FreeListNode* node = head;
	FreeListNode* prevNode = nullptr;
	FreeListNode* bestNode = nullptr;

	size_t padding = 0;

	while (node != nullptr)
	{
		padding = CalculatePaddingWithHeader((uintptr_t)node, (uintptr_t)alignment, sizeof(FreeListAllocationHeader));
		size_t requiredSpace = size + padding;

		size_t leftoverSpace = abs((int)node->blockSize - (int)requiredSpace);
		if (node->blockSize >= requiredSpace && leftoverSpace < smallestDiff)
		{
			smallestDiff = leftoverSpace;
			bestNode = node;
		}
		prevNode = node;
		node = node->next;
	}
	if (padding_) *padding_ = padding;
	if (prevNode_) *prevNode_ = prevNode;
	return bestNode;
}

void * FreeListAllocator::Alloc(size_t size, size_t alignment)
{
	size_t padding = 0;
	FreeListNode* prevNode = nullptr;
	FreeListNode* node = nullptr;
	size_t alignmentPadding, requiredSpace, remaining;
	FreeListAllocationHeader* headerPtr;

	if (size < sizeof(FreeListNode))
	{
		size = sizeof(FreeListNode);
	}

	if (alignment < 8)
	{
		alignment = 8;
	}

	if (policy == PlacementPolicy::FindBest)
	{
		node = FindBest(size, alignment, &padding, &prevNode);
	}
	else
	{
		node = FindFirst(size, alignment, &padding, &prevNode);
	}

	if (node == nullptr)
	{
		assert(0 && "Allocator has no free memory");
		return nullptr;
	}

	alignmentPadding = padding - sizeof(FreeListAllocationHeader);
	requiredSpace = size + padding;
	remaining = node->blockSize - requiredSpace;

	if (remaining > 0)
	{
		FreeListNode* newNode = (FreeListNode*)((char*)node + requiredSpace);
		newNode->blockSize = remaining;
		NodeInsert(&head, node, newNode);
	}

	NodeRemove(&head, prevNode, node);

	headerPtr = (FreeListAllocationHeader*)((char*)node + alignmentPadding);
	headerPtr->blockSize = requiredSpace;
	headerPtr->padding = alignmentPadding;

	used += requiredSpace;

	return (void*)((char*)headerPtr + sizeof(FreeListAllocationHeader));
}

void FreeListAllocator::Coalescence(FreeListNode * prevNode, FreeListNode * freeNode)
{
	if (freeNode->next != nullptr && (void*)((char*)freeNode + freeNode->blockSize) == freeNode->next)
	{
		freeNode->blockSize += freeNode->next->blockSize;
		NodeRemove(&head, freeNode, freeNode->next);
	}

	if (prevNode && prevNode->next != nullptr && (void*)((char*)prevNode + prevNode->blockSize) == freeNode)
	{
		prevNode->blockSize += freeNode->next->blockSize;
		NodeRemove(&head, prevNode, freeNode);
	}
}

void FreeListAllocator::Free(void * ptr)

{
	FreeListAllocationHeader* header;
	FreeListNode* freeNode;
	FreeListNode* node;
	FreeListNode* prevNode = nullptr;

	if (ptr == nullptr)
	{
		return;
	}

	header = (FreeListAllocationHeader*)((char*)ptr - sizeof(FreeListAllocationHeader));
	freeNode = (FreeListNode*)header;
	freeNode->blockSize = header->blockSize + header->padding;
	freeNode->next = nullptr;

	node = head;
	while (node != nullptr)
	{
		if (ptr < node)
		{
			NodeInsert(&head, prevNode, freeNode);
			break;
		}
		prevNode = node;
		node = node->next;
	}

	used -= freeNode->blockSize;

	Coalescence(prevNode, freeNode);

	if (freeNode < head)
	{
		head = freeNode;
	}
}

void FreeListAllocator::NodeInsert(FreeListNode ** phead, FreeListNode * prevNode, FreeListNode * newNode)
{
	if (prevNode == nullptr)
	{
		if (*phead != nullptr)
		{
			newNode->next = *phead;
		}
		else
		{
			*phead = newNode;
		}
	}
	else
	{
		if (prevNode->next == nullptr)
		{
			prevNode->next = newNode;
			newNode->next = nullptr;
		}
		else
		{
			newNode->next = prevNode->next;
			prevNode->next = newNode;
		}
	}
}

void FreeListAllocator::NodeRemove(FreeListNode ** phead, FreeListNode * prevNode, FreeListNode * delNode)
{
	if (prevNode == nullptr)
	{
		*phead = delNode->next;
	}
	else
	{
		prevNode->next = delNode->next;
	}
}

