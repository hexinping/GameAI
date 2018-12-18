#include "stateMachine/MinerWifeOwnedStates.h"
#include "Locations.h"
#include <string>
#include "cocos2d.h"
#include "common/misc/utils.h"
#include "common/time/CrudeTimer.h"
#include "common/messaging/MessageDispatcher.h"
#include "common/messaging/MessageTypes.h"
#include "EntityNames.h"


WifesGlobalState* WifesGlobalState::Instance()
{
	static WifesGlobalState instance;

	return &instance;
}


void WifesGlobalState::Execute(MinerWife* wife)
{
	//1 in 10 chance of needing the bathroom
	if (RandFloat() < 0.1)
	{
		wife->GetFSM()->ChangeState(VisitBathroom::Instance());

	}
}

bool WifesGlobalState::OnMessage(MinerWife* pWife, const Telegram& msg)
{
	string name = GetNameOfEntity(pWife->ID());
	switch (msg.Msg)
	{
	case Msg_HiHoneyImHome:
	{

		CCLOG("Message handled by %s:    at time %f", name.c_str(), Clock->GetCurrentTime());
		CCLOG("%s:    Hi honey. Let me make you some of mah fine country stew", name.c_str());

		pWife->GetFSM()->ChangeState(CookStew::Instance());
	}

	return true;

	}//end switch

	return false;
}

//---------------------------------------DoHouseWork

DoHouseWork* DoHouseWork::Instance()
{
	static DoHouseWork instance;

	return &instance;
}


void DoHouseWork::Enter(MinerWife* wife)
{
}


void DoHouseWork::Execute(MinerWife* wife)
{
	string name = GetNameOfEntity(wife->ID());
	switch (RandInt(0, 2))
	{
	case 0:
		CCLOG("%s:    Moppin' the floor", name.c_str());
		break;

	case 1:
		CCLOG("%s:     Washin' the dishes", name.c_str());
		break;

	case 2:
		CCLOG("%s:   Makin' the bed", name.c_str());
		break;
	}
}

void DoHouseWork::Exit(MinerWife* wife)
{
}

bool DoHouseWork::OnMessage(MinerWife* pWife, const Telegram& msg)
{
	return false;
}

//------------------------------------------------------------------------VisitBathroom
VisitBathroom* VisitBathroom::Instance()
{
	static VisitBathroom instance;

	return &instance;
}


void VisitBathroom::Enter(MinerWife* wife)
{

	string name = GetNameOfEntity(wife->ID());
	CCLOG("%s:     Walkin' to the can. Need to powda mah pretty li'lle nose", name.c_str());
}


void VisitBathroom::Execute(MinerWife* wife)
{

	string name = GetNameOfEntity(wife->ID());
	CCLOG("%s:      Ahhhhhh! Sweet relief!", name.c_str());

	wife->GetFSM()->RevertToPreviousState();
}

void VisitBathroom::Exit(MinerWife* wife)
{
	string name = GetNameOfEntity(wife->ID());
	CCLOG("%s:       Leavin' the Jon", name.c_str());
}

bool VisitBathroom::OnMessage(MinerWife* pWife, const Telegram& msg)
{
	return false;
}


CookStew* CookStew::Instance()
{
	static CookStew instance;

	return &instance;
}


void CookStew::Enter(MinerWife* wife)
{
	//if not already cooking put the stew in the oven
	if (!wife->Cooking())
	{
		string name = GetNameOfEntity(wife->ID());
		CCLOG("%s:       Putting the stew in the oven", name.c_str());
		//send a delayed message myself so that I know when to take the stew
		//out of the oven
		Dispatch->DispatchMessages(1.5,                  //time delay
			wife->ID(),           //sender ID
			wife->ID(),           //receiver ID
			Msg_StewReady,        //msg
			NO_ADDITIONAL_INFO);

		wife->SetCooking(true);
	}
}


void CookStew::Execute(MinerWife* wife)
{
	string name = GetNameOfEntity(wife->ID());
	CCLOG("%s:       Fussin' over food", name.c_str());
}

void CookStew::Exit(MinerWife* wife)
{
	string name = GetNameOfEntity(wife->ID());
	CCLOG("%s:       Puttin' the stew on the table", name.c_str());
}


bool CookStew::OnMessage(MinerWife* wife, const Telegram& msg)
{
	string name = GetNameOfEntity(wife->ID());
	switch (msg.Msg)
	{
	case Msg_StewReady:
	{
		

		CCLOG("Message received by %s:       at time: %f", name.c_str(), Clock->GetCurrentTime());
		CCLOG("%s:       StewReady! Lets eat", name.c_str());
		//let hubby know the stew is ready
		Dispatch->DispatchMessages(SEND_MSG_IMMEDIATELY,
			wife->ID(),
			ent_Miner_Bob,
			Msg_StewReady,
			NO_ADDITIONAL_INFO);

		wife->SetCooking(false);

		wife->GetFSM()->ChangeState(DoHouseWork::Instance());
	}

	return true;

	}//end switch

	return false;
}