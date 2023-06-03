#include "..\..\..\Headers\Engine\Managers\AssetManager.h"

bool AssetManager::InitializeAssetDatabase()
{
	std::string line;
	std::ifstream assetDatabaseFile("Resources/Engine/AssetDatabase.txt");
	if (assetDatabaseFile.is_open())
	{
		while (std::getline(assetDatabaseFile, line))
		{
			std::stringstream lineStream(line);
			std::string tempString;

			std::getline(lineStream, tempString, ',');
			unsigned int assetID = std::stoi(tempString);

			std::string assetPath;
			std::getline(lineStream, assetPath, ',');


			std::getline(lineStream, tempString);
			unsigned int assetType = std::stoi(tempString);

			gameAssets.push_back(GameAsset(assetID, assetPath, (AssetType)assetType));
		}
		isInitialized = true;
	}

	return isInitialized;
}
