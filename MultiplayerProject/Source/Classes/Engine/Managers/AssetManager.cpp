#include "Engine/Managers/AssetManager.h"
#include <unordered_map>
#include <assert.h>

CreateClassMetadata(AssetManager);

struct CompareCStrings
{
	bool operator()( const char* lhs, const char* rhs) const 
	{
		return std::strcmp(lhs, rhs) == 0;
	}
};

static std::unordered_map<char*, StaticAssetData, std::hash<std::string>, CompareCStrings> staticAssetData;

bool AssetManager::InitializeAssetDatabase()
{
	//std::string line;
	//std::ifstream assetDatabaseFile("Resources/Engine/AssetDatabase.txt");
	//if (assetDatabaseFile.is_open())
	//{
	//	while (std::getline(assetDatabaseFile, line))
	//	{
	//		std::stringstream lineStream(line);
	//		std::string tempString;
	//
	//		std::getline(lineStream, tempString, ',');
	//		unsigned int assetID = std::stoi(tempString);
	//
	//		std::string assetPath;
	//		std::getline(lineStream, assetPath, ',');
	//
	//
	//		std::getline(lineStream, tempString);
	//		unsigned int assetType = std::stoi(tempString);
	//
	//		softGameAssets.push_back(SoftGameAsset(assetID, assetPath/*, (AssetType)assetType*/));
	//	}
	//	isInitialized = true;
	//}

	return isInitialized;
}

void* AssetManager::InstantiateByName(char* name, void* addressToInstantiate)
{
	void* newAsset = nullptr;

	auto it = staticAssetData.find(name);
	assert(it != staticAssetData.end());
	
	newAsset = it->second.instanceCreateCallback(addressToInstantiate);

	return newAsset;
}

unsigned int AssetManager::GetAssetSizeByName(char* name)
{
	unsigned int assetSize = -1;

	auto it = staticAssetData.find(name);
	assert(it != staticAssetData.end());
	assetSize = it->second.size;
	
	return assetSize;
}

void AssetManager::AddStaticAssetData(char* name, StaticAssetData newStaticAssetData)
{
	staticAssetData.insert(std::pair<char*, StaticAssetData>(name, newStaticAssetData));
}
