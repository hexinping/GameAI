#pragma once

#include "enity/BaseGameEntity.h"
#include "stateMachine/StateMachine.h"
#include "common/2D/Vector2D.h"



class MovingEntity : public BaseGameEntity
{
public:
	MovingEntity();

	virtual ~MovingEntity(){ delete m_pStateMachine; }
	StateMachine<MovingEntity>*  GetFSM()const{ return m_pStateMachine; }

	void Update(float t);



	//so must this
	virtual bool  HandleMessage(const Telegram& msg);
protected:
	Vector2D m_vVelocity; //速度向量
	Vector2D m_vHeading;  //实体朝向向量
	Vector2D m_vSide;     //垂直于朝向的向量

	//质量
	double       m_dMass;

	//最大速度
	double       m_dMaxSpeed;

	//自己产生的最大动力
	double        m_dMaxForce;

	//旋转的最大速率（弧度每秒）
	double       m_dMaxTurnRate;

private:

	StateMachine<MovingEntity> * m_pStateMachine;

public:
	MovingEntity(Vector2D position, double radius, Vector2D velocity, double max_speed, Vector2D heading, double  mass, Vector2D scale, double turn_rate, double max_force) :BaseGameEntity(0, position, radius),
		m_vHeading(heading),
		m_vVelocity(velocity),
		m_dMass(mass),
		m_vSide(m_vHeading.Perp()),
		m_dMaxSpeed(max_speed),
		m_dMaxTurnRate(turn_rate),
		m_dMaxForce(max_force)
	{
		m_vScale = scale;
		m_pStateMachine = new StateMachine<MovingEntity>(this);
	}


	//accessors
	Vector2D  Velocity()const{ return m_vVelocity; }
	void      SetVelocity(const Vector2D& NewVel){ m_vVelocity = NewVel; }

	double     Mass()const{ return m_dMass; }

	Vector2D  Side()const{ return m_vSide; }

	double     MaxSpeed()const{ return m_dMaxSpeed; }
	void      SetMaxSpeed(double new_speed){ m_dMaxSpeed = new_speed; }

	double     MaxForce()const{ return m_dMaxForce; }
	void      SetMaxForce(double mf){ m_dMaxForce = mf; }

	bool      IsSpeedMaxedOut()const{ return m_dMaxSpeed*m_dMaxSpeed >= m_vVelocity.LengthSq(); }
	double     Speed()const{ return m_vVelocity.Length(); }
	double     SpeedSq()const{ return m_vVelocity.LengthSq(); }

	Vector2D  Heading()const{ return m_vHeading; }
	void      SetHeading(Vector2D new_heading);
	bool      RotateHeadingToFacePosition(Vector2D target);

	double     MaxTurnRate()const{ return m_dMaxTurnRate; }
	void      SetMaxTurnRate(double val){ m_dMaxTurnRate = val; }

};

