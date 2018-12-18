#pragma once
#include "common/messaging/Telegram.h"
class BaseGameEntity
{
public:
	BaseGameEntity(int id);
	virtual ~BaseGameEntity(){};

	virtual void  Update() = 0;

	int           ID()const{ return m_ID; }

	virtual bool  HandleMessage(const Telegram& msg) = 0;
private:
	int m_ID;
	static int m_iNextValidID;
	void SetID(int val);
};

