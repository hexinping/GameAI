#pragma once
#include "stateMachine/State.h"
#include "enity/MinerWife.h"
#include "common/messaging/Telegram.h"
#include <iostream>
using namespace std;



class WifesGlobalState : public State < MinerWife >
{
private:

	WifesGlobalState(){}

	//copy ctor and assignment should be private
	WifesGlobalState(const WifesGlobalState&);
	WifesGlobalState& operator=(const WifesGlobalState&);

public:

	static WifesGlobalState* Instance();

	virtual void Enter(MinerWife* wife){}

	virtual void Execute(MinerWife* wife);

	virtual void Exit(MinerWife* wife){}

	virtual bool OnMessage(MinerWife* pWife, const Telegram& msg);
};



class DoHouseWork : public State < MinerWife >
{

private:

	DoHouseWork(){}

	//copy ctor and assignment should be private
	DoHouseWork(const DoHouseWork&);
	DoHouseWork& operator=(const DoHouseWork&);

public:

	static DoHouseWork* Instance();

	virtual void Enter(MinerWife* wife);

	virtual void Execute(MinerWife* wife);

	virtual void Exit(MinerWife* wife);

	virtual bool OnMessage(MinerWife* pWife, const Telegram& msg);
};



//------------------------------------------------------------------------
//

//------------------------------------------------------------------------
class VisitBathroom : public State < MinerWife >
{
private:

	VisitBathroom(){}

	//copy ctor and assignment should be private
	VisitBathroom(const VisitBathroom&);
	VisitBathroom& operator=(const VisitBathroom&);

public:

	static VisitBathroom* Instance();

	virtual void Enter(MinerWife* wife);

	virtual void Execute(MinerWife* wife);

	virtual void Exit(MinerWife* wife);

	virtual bool OnMessage(MinerWife* pWife, const Telegram& msg);
};

class CookStew : public State < MinerWife >
{
private:

	CookStew(){}

	//copy ctor and assignment should be private
	CookStew(const CookStew&);
	CookStew& operator=(const CookStew&);

public:

	//this is a singleton
	static CookStew* Instance();

	virtual void Enter(MinerWife* wife);

	virtual void Execute(MinerWife* wife);

	virtual void Exit(MinerWife* wife);

	virtual bool OnMessage(MinerWife* pWife, const Telegram& msg);
};

