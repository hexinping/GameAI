#include "stateMachine/MinerWifeOwnedStates.h"
#include "Locations.h"
#include <string>
#include "cocos2d.h"
#include "common/misc/utils.h"

enum
{
	ent_Miner_Bob,

	ent_Elsa
};

inline std::string GetNameOfEntity(int n)
{
	switch (n)
	{
	case ent_Miner_Bob:

		return "Miner Bob";

	case ent_Elsa:

		return "Elsa";

	default:

		return "UNKNOWN!";
	}
}

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
	CCLOG("%s:     Walkin' to the goldmine", name.c_str());
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