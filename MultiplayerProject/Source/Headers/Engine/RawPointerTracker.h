#pragma once

#include "Allocators/FreeListAllocator.h"

struct RawPointerHandle
{
	unsigned int index;
	unsigned int offset;
};

class RawPointerTrackableObject
{
public:
	RawPointerHandle* GetRawPointerHandle() { return rawPointerHandle; }
protected:
	virtual bool Initialize();
	virtual ~RawPointerTrackableObject();
	RawPointerHandle* rawPointerHandle;
};

class WeakRawPointerHandle
{
public:
	WeakRawPointerHandle(RawPointerTrackableObject* rawPointerTrackableObject) : 
		cachedHandle(*rawPointerTrackableObject->GetRawPointerHandle()),
		handleOffset(cachedHandle.offset)
	{
	}

	WeakRawPointerHandle(RawPointerHandle inHandle)  : 
		cachedHandle(inHandle),
		handleOffset(cachedHandle.offset)
	{
	}

	bool IsValid();
private:
	RawPointerHandle cachedHandle;
	unsigned int handleOffset;
};

class RawPointerTracker
{
public:
	void Initialize() { allocator.Init(10000); }
	RawPointerHandle* CreateTrackedRawPointer(void* pointerToTrack);

	void RemoveTrackedRawPointer(RawPointerTrackableObject* trackableObjectToRemove);

	RawPointerHandle GetHandleByOffset(unsigned int offset) { return *(RawPointerHandle*)(allocator.GetDataAtOffset(offset)); }
private:
	FreeListAllocator allocator;
	unsigned int largestIndex;
};