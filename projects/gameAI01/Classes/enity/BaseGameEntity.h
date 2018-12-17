#pragma once
class BaseGameEntity
{
public:
	BaseGameEntity(int id);
	virtual ~BaseGameEntity(){};

	virtual void  Update() = 0;

	int           ID()const{ return m_ID; }
private:
	int m_ID;
	static int m_iNextValidID;
	void SetID(int val);
};

