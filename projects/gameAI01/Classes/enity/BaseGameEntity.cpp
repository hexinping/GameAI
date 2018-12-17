#include "BaseGameEntity.h"
#include <cassert>

BaseGameEntity::BaseGameEntity(int id)
{
	SetID(id);
}


void BaseGameEntity::SetID(int val)
{
	m_ID = val;
}