#include "Miner.h"
#include "stateMachine/MinerOwnedStates.h"
#include <cassert>

Miner::Miner() :BaseGameEntity(),
m_Location(shack),
m_iGoldCarried(0),
m_iMoneyInBank(0),
m_iThirst(0),
m_iFatigue(0)
{
	m_pStateMachine = new StateMachine<Miner>(this);
	m_pStateMachine->SetCurrentState(GoHomeAndSleepTilRested::Instance());
}


//-----------------------------------------------------------------------------
void Miner::AddToGoldCarried(const int val)
{
	m_iGoldCarried += val;

	if (m_iGoldCarried < 0) m_iGoldCarried = 0;
}


//-----------------------------------------------------------------------------
void Miner::AddToWealth(const int val)
{
	m_iMoneyInBank += val;

	if (m_iMoneyInBank < 0) m_iMoneyInBank = 0;
}


//-----------------------------------------------------------------------------
bool Miner::Thirsty()const
{
	if (m_iThirst >= ThirstLevel){ return true; }

	return false;
}


//-----------------------------------------------------------------------------
void Miner::Update()
{
	m_iThirst += 1;

	m_pStateMachine->Update();
}


//-----------------------------------------------------------------------------
bool Miner::Fatigued()const
{
	if (m_iFatigue > TirednessThreshold)
	{
		return true;
	}

	return false;
}

bool Miner::HandleMessage(const Telegram& msg)
{
	return m_pStateMachine->HandleMessage(msg);
}
