#pragma once
#include "2d/CCSprite.h"
#include "enity/Vehicle.h"
#include "common/2D/Vector2D.h"
#include "GameWorld.h"



enum EState
{
	None,
	Seek, //靠近
	Flee, //离开
};


using namespace cocos2d;
class VehicleSprite :public Sprite
{
public:
	VehicleSprite(Vector2D pos, Vector2D targetPos, GameWorld *gameWorld);
	~VehicleSprite();
	static VehicleSprite* create(const std::string& file, Vector2D pos, Vector2D targetPos, GameWorld *gameWorld);

	bool initWithFile(const std::string& file);

	void updateS(float dt);

	void setHead(Vector2D head);
	Vector2D calculate();

	Vector2D getPos(){ return m_vPos; }

	//返回靠近的力
	Vector2D seekForce(Vector2D TargetPos);
	bool isSeekOver();

	//返回离开的力
	Vector2D fleeForce(Vector2D TargetPos);
public:
	Vehicle *m_pVehicle;

	EState m_state;

	//运动学相关变量
	float m_dMass = 1.0; //质量
	float m_dMaxSpeed = 150; //最大速度
	float m_maxSteeringForce = 400; //最大操作力

	Vector2D m_targetPos;
	Vector2D m_startPos;

	Vector2D m_vVelocity; //交通体的速度
	Vector2D m_vPos;      //交通体的位置
	Vector2D m_vHeading;  //朝向
	Vector2D m_vSide;     //垂直朝向的平面
};

