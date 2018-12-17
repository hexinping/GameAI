#pragma once
#include "stateMachine/State.h"
#include "enity/MinerWife.h"
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

};

