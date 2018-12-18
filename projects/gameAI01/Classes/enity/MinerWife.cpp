#include "MinerWife.h"
#include "stateMachine/MinerWifeOwnedStates.h"
#include "enity/EntityManager.h"
#include <cassert>

MinerWife::MinerWife(int id) :BaseGameEntity(id),
m_Location(shack),
m_bCooking(false)
{
	m_pStateMachine = new StateMachine<MinerWife>(this);
	m_pStateMachine->SetCurrentState(DoHouseWork::Instance());
	m_pStateMachine->SetGlobalState(WifesGlobalState::Instance());
	EntityMgr->RegisterEntity(this);
}


void MinerWife::Update()
{
	m_pStateMachine->Update();
}

bool MinerWife::HandleMessage(const Telegram& msg)
{
	return m_pStateMachine->HandleMessage(msg);
}