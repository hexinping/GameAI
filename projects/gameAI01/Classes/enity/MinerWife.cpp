#include "MinerWife.h"
#include "stateMachine/MinerWifeOwnedStates.h"
#include <cassert>

MinerWife::MinerWife(int id) :BaseGameEntity(id),
m_Location(shack)
{
	m_pStateMachine = new StateMachine<MinerWife>(this);
	m_pStateMachine->SetCurrentState(DoHouseWork::Instance());
	m_pStateMachine->SetGlobalState(WifesGlobalState::Instance());
}


void MinerWife::Update()
{
	m_pStateMachine->Update();
}