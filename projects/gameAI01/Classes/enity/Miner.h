#pragma once

#include "enity/BaseGameEntity.h"
#include "Locations.h"
#include "stateMachine/State.h"
//class State;


//the amount of gold a miner must have before he feels comfortable
const int ComfortLevel = 5;
//the amount of nuggets a miner can carry
const int MaxNuggets = 3;
//above this value a miner is thirsty
const int ThirstLevel = 5;
//above this value a miner is sleepy
const int TirednessThreshold = 5;

class Miner : public BaseGameEntity
{
public:
	Miner(int id);

	void Update();

	void ChangeState(State<Miner> *newState);
	void RevertToPreviousState();

	location_type Location() const{ return m_Location; }
	void  ChangeLocation(const location_type loc){ m_Location = loc; }
	
	int   GoldCarried() const{ return m_iGoldCarried; }
	void  SetGoldCarried(const int val) { m_iGoldCarried = val; }
	void  AddToGoldCarried(const int val);
	bool  PocketsFull()const{ return m_iGoldCarried >= MaxNuggets; }

	bool  Fatigued() const;
	void  DecreaseFatigue() { m_iFatigue -= 1; }
	void  IncreaseFatigue(){ m_iFatigue += 1; }

	int   Wealth()const{ return m_iMoneyInBank; }
	void  SetWealth(const int val){ m_iMoneyInBank = val; }
	void  AddToWealth(const int val);

	bool  Thirsty()const;
	void  BuyAndDrinkAWhiskey(){ m_iThirst = 0; m_iMoneyInBank -= 2; }

public:
private:
	State<Miner>* m_pCurrentState;
	State<Miner>* m_pPreviousState;//前置状态
	State<Miner>* m_pGlobalState;



	location_type m_Location;

	//how many nuggets the miner has in his pockets
	int                   m_iGoldCarried;

	int                   m_iMoneyInBank;

	//the higher the value, the thirstier the miner
	int                   m_iThirst;

	//the higher the value, the more tired the miner
	int                   m_iFatigue;

};

