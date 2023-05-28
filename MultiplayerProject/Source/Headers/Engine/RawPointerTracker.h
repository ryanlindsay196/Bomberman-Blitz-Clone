#pragma once

#include "Allocators/FreeListAllocator.h"

struct RawPointerHandle
{
	int index;
	void* pointer;
};

class RawPointerTrackableObject
{
public:
	RawPointerHandle* GetRawPointerHandle() { return &rawPointerHandle; }
protected:
	virtual bool Initialize();
	virtual ~RawPointerTrackableObject();
	RawPointerHandle rawPointerHandle;
};

class RawPointerTracker
{
public:
	void Initialize() { allocator.Init(10000); }
	RawPointerHandle* CreateTrackedRawPointer(void* pointerToTrack);

	void RemoveTrackedRawPointer(RawPointerTrackableObject* trackableObjectToRemove);
private:
	FreeListAllocator allocator;
	unsigned int largestIndex;
};