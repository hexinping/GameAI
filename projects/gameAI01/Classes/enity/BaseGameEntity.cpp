#include "BaseGameEntity.h"

BaseGameEntity::BaseGameEntity(int id)
{
	SetID(id);
}


void BaseGameEntity::SetID(int val)
{
	m_ID = val;
}