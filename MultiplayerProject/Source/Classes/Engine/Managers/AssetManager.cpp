#include "Engine\Managers\AssetManager.h"
#include <assert.h>

static std::vector<StaticAssetData>& GetStaticAssetData()
{
	static std::vector<StaticAssetData> staticAssetData;
	return staticAssetData;
}

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

void* AssetManager::InstantiateByStaticID(unsigned int staticID, void* addressToInstantiate)
{
	void* newAsset = nullptr;

	if (staticID < GetStaticAssetData().size())
	{
		newAsset = GetStaticAssetData()[staticID].instanceCreateCallback(addressToInstantiate);
	}

	return newAsset;
}

unsigned int AssetManager::GetAssetSizeByStaticID(unsigned int staticID)
{
	unsigned int assetSize = -1;
	assert(staticID < GetStaticAssetData().size());
	assetSize = GetStaticAssetData()[staticID].size;
	return assetSize;
}

void AssetManager::AddStaticAssetData(StaticAssetData newStaticAssetData)
{
	GetStaticAssetData().push_back(newStaticAssetData);
}
