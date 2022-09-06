#include "SDL.h"
#include "SDL_image.h"

#include "Game/Entity.h"
#include "Engine/Rendering/Renderer.h"

bool Entity::Initialize(const char *imagePath, float inWidth, float inHeight, float inPosX, float inPosY)
{
	networkID = -1;
	width = inWidth;
	height = inHeight;
	posX = inPosX;
	posY = inPosY;

	GameManager& gameManager = GameManager::Get();
	texture = gameManager.GetRenderer()->LoadTexture(imagePath);

	CreateVariableMetadata(Entity, posX, AuthorityType::OwningClient)
	CreateVariableMetadata(Entity, posY, AuthorityType::OwningClient)

	return texture != nullptr;
}

void Entity::SerializeNetworkedVariables()
{
	static GameManager& gameManager = GameManager::Get();
	static NetworkManager* networkManager = gameManager.GetNetworkManager();
	
	std::vector<NetworkedVariableData*> dataToSerialize;
	unsigned int totalBytesToSerialize = 0;
	
	for (NetworkedVariableData& var : networkedVariables)
	{
		char* updatedData = (char*)this + (var.offset);
		bool isEqual = true;
		for (int i = 0; i < var.size; ++i)
		{
			if (var.data[i] != updatedData[i])
			{
				isEqual = false;
				break;
			}
		}
	
		bool variableAuthorityMatchesServerState =
			(var.authorityType == AuthorityType::Server && networkManager->GetIsServer()) ||
			(var.authorityType == AuthorityType::OwningClient && !networkManager->GetIsServer());
	
		if (!isEqual && variableAuthorityMatchesServerState)
		{
			memcpy(&var.data, updatedData, var.size);
			dataToSerialize.push_back(&var);
			totalBytesToSerialize += sizeof(NetworkedVariableData) - sizeof(var.data) + var.size;
		}
	}
	
	if (totalBytesToSerialize == 0)
	{
		return;
	}
	
	if (!networkManager->CanSerializeNumberOfBytes(sizeof(networkID) + sizeof(totalBytesToSerialize) + totalBytesToSerialize))
	{
		networkManager->SendSerializedData();
	}
	if (networkManager->IsOutputStreamEmpty())
	{
		RakNet::MessageID messID = (RakNet::MessageID)ID_UPDATE_ENTITY;
		networkManager->Serialize(&messID, sizeof(messID));
	}
	
	networkManager->Serialize(&networkID, sizeof(networkID));
	networkManager->Serialize(&totalBytesToSerialize, sizeof(totalBytesToSerialize));
	for (NetworkedVariableData* var : dataToSerialize)
	{
		assert(networkManager->Serialize(var, sizeof(NetworkedVariableData) - sizeof(var->data) + var->size));
	}
}

void Entity::DeserializeNetworkedVariable(unsigned int offset, unsigned int size, char* dataSrc)
{
	char* dataDest = (char*)this + (offset);
	memcpy(dataDest, dataSrc, size);
}

void Entity::Update(float deltaTime)
{
	SerializeNetworkedVariables();
}

void Entity::Render()
{
	SDL_Rect destRect;
	destRect.x = posX;
	destRect.y = posY;
	destRect.w = width;
	destRect.h = height;

	GameManager& gameManager = GameManager::Get();
	gameManager.GetRenderer()->UpdateRender(texture, nullptr, &destRect);
}

void Entity::Destroy()
{
	SDL_DestroyTexture(texture);
}
