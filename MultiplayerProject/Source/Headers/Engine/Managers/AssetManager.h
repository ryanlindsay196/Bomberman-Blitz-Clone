#pragma once
#include "Engine/Reflection/Reflection.h"

struct StaticAssetData
{
	unsigned int size;
	void*(*instanceCreateCallback)(void*);
};

template <typename T>
class  StaticAsset
{
public:
	static StaticAsset& Get() 
	{
		static StaticAsset instance;
		return instance;
	}

private:
	StaticAsset()
	{
		StaticAssetData data = { sizeof(T), &StaticAsset<T>::CreateInstance };
		AssetManager::AddStaticAssetData(T::GetClassName(), data);
		//AssetManager::AddSoftObject();
	}
	StaticAsset(StaticAsset const&) = delete;
	void operator=(StaticAsset const&) = delete;

	static void* CreateInstance(void* addressToInstantiate)
	{
		void* asset = nullptr;
		if (addressToInstantiate)
		{
			asset = new(addressToInstantiate) T();
		}
		else
		{
			asset = new T();
		}
		return asset;
	}
};



#define RegisterStaticAsset(T) StaticAsset<T>& T##StaticAsset = StaticAsset<T>::Get()

//struct SoftGameAsset
//{
//	SoftGameAsset(unsigned int inID, std::string inPath) :
//		id(inID),
//		path(inPath),
//		gameAsset(nullptr)
//	{
//
//	}
//
//	unsigned int id;
//	std::string path;
//	void* gameAsset;
//};

class AssetManager
{
	CreateClassMetadata(AssetManager)
public:
	template<typename T>
	friend class StaticAsset;

	bool InitializeAssetDatabase();

	void* InstantiateByName(char* name, void* addressToInstantiate);
	unsigned int GetAssetSizeByName(char* name);
public:

	bool GetIsInitialized() { return isInitialized; }

private:
	static void AddStaticAssetData(char* name, StaticAssetData newStaticAssetData);

	bool isInitialized;
};
