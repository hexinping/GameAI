#include "MinerWife.h"
#include "stateMachine/MinerWifeOwnedStates.h"
#include <cassert>

MinerWife::MinerWife() :BaseGameEntity(),
m_Location(shack),
m_bCooking(false)
{
	m_pStateMachine = new StateMachine<MinerWife>(this);
	m_pStateMachine->SetCurrentState(DoHouseWork::Instance());
	m_pStateMachine->SetGlobalState(WifesGlobalState::Instance());
}


void MinerWife::Update()
{
	m_pStateMachine->Update();
}

bool MinerWife::HandleMessage(const Telegram& msg)
{
	return m_pStateMachine->HandleMessage(msg);
}