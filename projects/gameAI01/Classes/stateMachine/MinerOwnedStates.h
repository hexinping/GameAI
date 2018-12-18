#pragma once
#include "stateMachine/State.h"
#include "enity/Miner.h"
#include "common/messaging/Telegram.h"
#include <iostream>
using namespace std;



class EnterMineAndDigForNugget : public State<Miner>
{
private:

	EnterMineAndDigForNugget(){}

	//copy ctor and assignment should be private
	EnterMineAndDigForNugget(const EnterMineAndDigForNugget&);
	EnterMineAndDigForNugget& operator=(const EnterMineAndDigForNugget&);

public:

	//this is a singleton
	static EnterMineAndDigForNugget* Instance();

	virtual void Enter(Miner* miner);

	virtual void Execute(Miner* miner);

	virtual void Exit(Miner* miner);


	virtual bool OnMessage(Miner* pMiner, const Telegram& msg);
};



//------------------------------------------------------------------------
//
//  Entity will go to a bank and deposit any nuggets he is carrying. If the 
//  miner is subsequently wealthy enough he'll walk home, otherwise he'll
//  keep going to get more gold
//------------------------------------------------------------------------
class VisitBankAndDepositGold : public State<Miner>
{
private:

	VisitBankAndDepositGold(){}

	//copy ctor and assignment should be private
	VisitBankAndDepositGold(const VisitBankAndDepositGold&);
	VisitBankAndDepositGold& operator=(const VisitBankAndDepositGold&);

public:

	//this is a singleton
	static VisitBankAndDepositGold* Instance();

	virtual void Enter(Miner* miner);

	virtual void Execute(Miner* miner);

	virtual void Exit(Miner* miner);

	virtual bool OnMessage(Miner* pMiner, const Telegram& msg);
};

class GoHomeAndSleepTilRested :public State<Miner>
{
private:

	GoHomeAndSleepTilRested(){}

	//copy ctor and assignment should be private
	GoHomeAndSleepTilRested(const GoHomeAndSleepTilRested&);
	GoHomeAndSleepTilRested& operator=(const GoHomeAndSleepTilRested&);

public:

	//this is a singleton
	static GoHomeAndSleepTilRested* Instance();

	virtual void Enter(Miner* miner);

	virtual void Execute(Miner* miner);

	virtual void Exit(Miner* miner);

	virtual bool OnMessage(Miner* pMiner, const Telegram& msg);
};


//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
class QuenchThirst : public State<Miner>
{
private:

	QuenchThirst(){}

	//copy ctor and assignment should be private
	QuenchThirst(const QuenchThirst&);
	QuenchThirst& operator=(const QuenchThirst&);

public:

	//this is a singleton
	static QuenchThirst* Instance();

	virtual void Enter(Miner* miner);

	virtual void Execute(Miner* miner);

	virtual void Exit(Miner* miner);

	virtual bool OnMessage(Miner* pMiner, const Telegram& msg);
};

class EatStew : public State < Miner >
{
private:

	EatStew(){}

	//copy ctor and assignment should be private
	EatStew(const EatStew&);
	EatStew& operator=(const EatStew&);

public:

	//this is a singleton
	static EatStew* Instance();

	virtual void Enter(Miner* miner);

	virtual void Execute(Miner* miner);

	virtual void Exit(Miner* miner);

	virtual bool OnMessage(Miner* pMiner, const Telegram& msg);
};

