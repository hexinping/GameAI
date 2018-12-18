#pragma once

#include "enity/BaseGameEntity.h"
#include "Locations.h"
#include "stateMachine/StateMachine.h"



class MinerWife : public BaseGameEntity
{
public:
	MinerWife();

	~MinerWife(){ delete m_pStateMachine; }
	StateMachine<MinerWife>*  GetFSM()const{ return m_pStateMachine; }

	void Update();


	//----------------------------------------------------accessors
	location_type Location()const{ return m_Location; }
	void          ChangeLocation(const location_type loc){ m_Location = loc; }


	bool          Cooking()const{ return m_bCooking; }
	void          SetCooking(bool val){ m_bCooking = val; }

	//so must this
	virtual bool  HandleMessage(const Telegram& msg);

private:

	StateMachine<MinerWife> * m_pStateMachine;

	location_type m_Location;

	//is she presently cooking?
	bool            m_bCooking;

};

