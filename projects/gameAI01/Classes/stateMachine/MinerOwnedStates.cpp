#include "stateMachine/MinerOwnedStates.h"
#include "Locations.h"
#include <string>
#include "cocos2d.h"
#include "EntityNames.h"
#include "common/messaging/MessageDispatcher.h"
#include "common/messaging/MessageTypes.h"
#include "common/time/CrudeTimer.h"


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
		pMiner->GetFSM()->ChangeState(VisitBankAndDepositGold::Instance());
		return;
	}

	if (pMiner->Thirsty())
	{
		pMiner->GetFSM()->ChangeState(QuenchThirst::Instance());
		return;
	}
}


void EnterMineAndDigForNugget::Exit(Miner* pMiner)
{
	string name = GetNameOfEntity(pMiner->ID());
	CCLOG("%s:     Ah'm leavin' the goldmine with mah pockets full o' sweet gold", name.c_str());
}

bool EnterMineAndDigForNugget::OnMessage(Miner* pMiner, const Telegram& msg)
{
	//send msg to global message handler
	return false;
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
		pMiner->GetFSM()->ChangeState(GoHomeAndSleepTilRested::Instance());
	}

	//otherwise get more gold
	else
	{
		pMiner->GetFSM()->ChangeState(EnterMineAndDigForNugget::Instance());
	}
}


void VisitBankAndDepositGold::Exit(Miner* pMiner)
{
	
	string name = GetNameOfEntity(pMiner->ID());
	CCLOG("%s:     Leavin' the bank", name.c_str());
}

bool VisitBankAndDepositGold::OnMessage(Miner* pMiner, const Telegram& msg)
{
	//send msg to global message handler
	return false;
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

		//let the wife know I'm home
		Dispatch->DispatchMessages(SEND_MSG_IMMEDIATELY, //time delay
			pMiner->ID(),        //ID of sender
			ent_Elsa,            //ID of recipient
			Msg_HiHoneyImHome,   //the message
			NO_ADDITIONAL_INFO);
	}
}

void GoHomeAndSleepTilRested::Execute(Miner* pMiner)
{
	string name = GetNameOfEntity(pMiner->ID());
	if (!pMiner->Fatigued())
	{
		
		CCLOG("%s:     What a God darn fantastic nap! Time to find more gold", name.c_str());
		pMiner->GetFSM()->ChangeState(EnterMineAndDigForNugget::Instance());
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

bool GoHomeAndSleepTilRested::OnMessage(Miner* pMiner, const Telegram& msg)
{
	string name = GetNameOfEntity(pMiner->ID());
	switch (msg.Msg)
	{
	case Msg_StewReady:

		CCLOG("Message handled by %s:  at time: %f", name.c_str(), Clock->GetCurrentTime());
		CCLOG("%s:  : Okay Hun, ahm a comin'!", name.c_str());
		pMiner->GetFSM()->ChangeState(EatStew::Instance());

		return true;

	}//end switch

	return false; //send message to global message handler
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

		pMiner->GetFSM()->ChangeState(EnterMineAndDigForNugget::Instance());
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

bool QuenchThirst::OnMessage(Miner* pMiner, const Telegram& msg)
{
	//send msg to global message handler
	return false;
}

EatStew* EatStew::Instance()
{
	static EatStew instance;

	return &instance;
}


void EatStew::Enter(Miner* pMiner)
{
	string name = GetNameOfEntity(pMiner->ID());
	CCLOG("%s:     Smells Reaaal goood Elsa!", name.c_str());
}

void EatStew::Execute(Miner* pMiner)
{
	string name = GetNameOfEntity(pMiner->ID());
	CCLOG("%s:     Tastes real good too!", name.c_str());
	pMiner->GetFSM()->RevertToPreviousState();
}

void EatStew::Exit(Miner* pMiner)
{
	string name = GetNameOfEntity(pMiner->ID());
	CCLOG("%s:     Thankya li'lle lady. Ah better get back to whatever ah wuz doin'", name.c_str());
}


bool EatStew::OnMessage(Miner* pMiner, const Telegram& msg)
{
	//send msg to global message handler
	return false;
}

