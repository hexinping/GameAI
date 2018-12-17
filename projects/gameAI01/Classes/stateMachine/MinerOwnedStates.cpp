#include "stateMachine/MinerOwnedStates.h"
#include "Locations.h"
#include <string>
#include "cocos2d.h"

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


EnterMineAndDigForNugget* EnterMineAndDigForNugget::Instance()
{
	static EnterMineAndDigForNugget instance;

	return &instance;
}

void EnterMineAndDigForNugget::Enter(Miner* pMiner)
{
	//if the miner is not already located at the goldmine, he must
	//change location to the gold mine
	if (pMiner->Location() != goldmine)
	{
		string name = GetNameOfEntity(pMiner->ID());
		CCLOG("%s:     Walkin' to the goldmine", name.c_str());
		pMiner->ChangeLocation(goldmine);
	}
}


void EnterMineAndDigForNugget::Execute(Miner* pMiner)
{
	//the miner digs for gold until he is carrying in excess of MaxNuggets. 
	//If he gets thirsty during his digging he packs up work for a while and 
	//changes state to go to the saloon for a whiskey.
	pMiner->AddToGoldCarried(1);

	pMiner->IncreaseFatigue();

	string name = GetNameOfEntity(pMiner->ID());
	CCLOG("%s:     Pickin' up a nugget", name.c_str());
	//if enough gold mined, go and put it in the bank
	if (pMiner->PocketsFull())
	{
		pMiner->ChangeState(VisitBankAndDepositGold::Instance());
		return;
	}

	if (pMiner->Thirsty())
	{
		pMiner->ChangeState(QuenchThirst::Instance());
		return;
	}
}


void EnterMineAndDigForNugget::Exit(Miner* pMiner)
{
	string name = GetNameOfEntity(pMiner->ID());
	CCLOG("%s:     Ah'm leavin' the goldmine with mah pockets full o' sweet gold", name.c_str());
}

//----------------------------------------methods for VisitBankAndDepositGold

VisitBankAndDepositGold* VisitBankAndDepositGold::Instance()
{
	static VisitBankAndDepositGold instance;

	return &instance;
}


void VisitBankAndDepositGold::Enter(Miner* pMiner)
{
	//on entry the miner makes sure he is located at the bank
	if (pMiner->Location() != bank)
	{
		string name = GetNameOfEntity(pMiner->ID());
		CCLOG("%s:     Goin' to the bank. Yes siree", name.c_str());
		pMiner->ChangeLocation(bank);
	}
}


void VisitBankAndDepositGold::Execute(Miner* pMiner)
{

	//deposit the gold
	pMiner->AddToWealth(pMiner->GoldCarried());

	pMiner->SetGoldCarried(0);

	string name = GetNameOfEntity(pMiner->ID());
	CCLOG("%s:     Depositing gold. Total savings now: %d", name.c_str(), pMiner->Wealth());
	//wealthy enough to have a well earned rest?
	if (pMiner->Wealth() >= ComfortLevel)
	{
		string name = GetNameOfEntity(pMiner->ID());
		CCLOG("%s:     WooHoo! Rich enough for now. Back home to mah li'lle lady", name.c_str());
		pMiner->ChangeState(GoHomeAndSleepTilRested::Instance());
	}

	//otherwise get more gold
	else
	{
		pMiner->ChangeState(EnterMineAndDigForNugget::Instance());
	}
}


void VisitBankAndDepositGold::Exit(Miner* pMiner)
{
	
	string name = GetNameOfEntity(pMiner->ID());
	CCLOG("%s:     Leavin' the bank", name.c_str());
}


//----------------------------------------methods for GoHomeAndSleepTilRested
GoHomeAndSleepTilRested* GoHomeAndSleepTilRested::Instance()
{
	static GoHomeAndSleepTilRested instance;

	return &instance;
}


void GoHomeAndSleepTilRested::Enter(Miner* pMiner)
{
	if (pMiner->Location() != shack)
	{
		string name = GetNameOfEntity(pMiner->ID());
		CCLOG("%s:     Walkin home", name.c_str());
		pMiner->ChangeLocation(shack);
	}
}

void GoHomeAndSleepTilRested::Execute(Miner* pMiner)
{
	string name = GetNameOfEntity(pMiner->ID());
	if (!pMiner->Fatigued())
	{
		
		CCLOG("%s:     What a God darn fantastic nap! Time to find more gold", name.c_str());
		pMiner->ChangeState(EnterMineAndDigForNugget::Instance());
	}
	else
	{
		//sleep
		pMiner->DecreaseFatigue();
		CCLOG("%s:     ZZZZ... ", name.c_str());
	}
}

void GoHomeAndSleepTilRested::Exit(Miner* pMiner)
{
	string name = GetNameOfEntity(pMiner->ID());
	CCLOG("%s:     Leaving the house", name.c_str());
}


//------------------------------------------------methods for QuenchThirst

QuenchThirst* QuenchThirst::Instance()
{
	static QuenchThirst instance;

	return &instance;
}

void QuenchThirst::Enter(Miner* pMiner)
{
	if (pMiner->Location() != saloon)
	{
		pMiner->ChangeLocation(saloon);

		string name = GetNameOfEntity(pMiner->ID());
		CCLOG("%s:     Boy, ah sure is thusty! Walking to the saloon", name.c_str());
	}
}

void QuenchThirst::Execute(Miner* pMiner)
{
	if (pMiner->Thirsty())
	{
		pMiner->BuyAndDrinkAWhiskey();

		string name = GetNameOfEntity(pMiner->ID());
		CCLOG("%s:     That's mighty fine sippin liquer", name.c_str());

		pMiner->ChangeState(EnterMineAndDigForNugget::Instance());
	}

	else
	{
		CCLOG("ERROR!ERROR!ERROR");
	}
}

void QuenchThirst::Exit(Miner* pMiner)
{
	string name = GetNameOfEntity(pMiner->ID());
	CCLOG("%s:     Leaving the saloon, feelin' good", name.c_str());
}
