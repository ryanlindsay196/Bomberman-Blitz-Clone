#pragma once

#include "Engine/UI/BaseWidget.h"
#include "Engine/Allocators/FreeListAllocator.h"

class WidgetTree
{
public:
	void Initialize(size_t widgetAllocatorSizeInBytes);

	template<class T>
	T* CreateWidget(T* parentWidget)
	{
		void* newPtr = allocator.Alloc(sizeof(T), 8);
		T* newWidget = new(newPtr) T;
#ifdef RunDebugMode
		static_cast<T*>(newWidget);
#endif

		if (parentWidget)
		{
			parentWidget->AddChild(newWidget);
			newWidget->SetParent(parentWidget);
		}
		else
		{
			rootWidget.AddChild(newWidget);
			newWidget->SetParent(nullptr);
		}

		return newWidget;
	}

	void RemoveWidget(BaseWidget* widgetToRemove);

private:
	BaseWidget rootWidget;
	FreeListAllocator allocator;
};