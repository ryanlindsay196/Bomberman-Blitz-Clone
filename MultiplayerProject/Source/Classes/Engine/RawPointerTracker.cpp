#include "Engine/RawPointerTracker.h"
#include "Game/GameManager.h"

bool RawPointerTrackableObject::Initialize()
{
	RawPointerTracker* rawPointerTracker = GameManager::GetRawPointerTracker();
	assert(rawPointerTracker);
	rawPointerHandle = rawPointerTracker->CreateTrackedRawPointer(this);
	return true;
}

RawPointerTrackableObject::~RawPointerTrackableObject()
{
	RawPointerTracker* rawPointerTracker = GameManager::GetRawPointerTracker();

	if (rawPointerTracker)
	{
		rawPointerTracker->RemoveTrackedRawPointer(this);
	}
}

RawPointerHandle* RawPointerTracker::CreateTrackedRawPointer(void * pointerToTrack)
{
	++largestIndex;
	RawPointerHandle* handle = (RawPointerHandle*)allocator.Alloc(sizeof(RawPointerHandle), 32);
	handle->index = largestIndex;
	handle->offset = ((char*)handle - allocator.GetDataAtOffset(0));
	return handle;
}

void RawPointerTracker::RemoveTrackedRawPointer(RawPointerTrackableObject * trackableObjectToRemove)
{	
	//Make the contents of the handle all equal 0;
	RawPointerHandle* handleToClear = trackableObjectToRemove->GetRawPointerHandle();
	
	if (!handleToClear)
	{
		return;
	}

	*handleToClear = {};
	allocator.Free(handleToClear);
}

bool WeakRawPointerHandle::IsValid()
{
	RawPointerHandle handle = GameManager::GetRawPointerTracker()->GetHandleByOffset(handleOffset);
	return handle.index == cachedHandle.index;
}
