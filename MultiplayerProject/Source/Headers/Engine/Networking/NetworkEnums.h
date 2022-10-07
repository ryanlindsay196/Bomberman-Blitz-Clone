#pragma once

#include "MessageIdentifiers.h"
enum class AuthorityType
{
	OwningClient,
	Server
};

enum GameMessages
{
	ID_CREATE_ENTITY = ID_USER_PACKET_ENUM + 1,
	ID_INITIALIZE_NEW_CLIENT,
	ID_UPDATE_ENTITY,
	ID_DESTROY_ENTITY,
	ID_CALL_RPC
};