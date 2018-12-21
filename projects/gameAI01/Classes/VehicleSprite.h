#pragma once
#include "2d/CCSprite.h"
#include "enity/Vehicle.h"
#include "common/2D/Vector2D.h"
#include "GameWorld.h"



enum EState
{
	None,
	Seek, //����
	Flee, //�뿪
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

	//���ؿ�������
	Vector2D seekForce(Vector2D TargetPos);
	bool isSeekOver();

	//�����뿪����
	Vector2D fleeForce(Vector2D TargetPos);
public:
	Vehicle *m_pVehicle;

	EState m_state;

	//�˶�ѧ��ر���
	float m_dMass = 1.0; //����
	float m_dMaxSpeed = 150; //����ٶ�
	float m_maxSteeringForce = 400; //��������

	Vector2D m_targetPos;
	Vector2D m_startPos;

	Vector2D m_vVelocity; //��ͨ����ٶ�
	Vector2D m_vPos;      //��ͨ���λ��
	Vector2D m_vHeading;  //����
	Vector2D m_vSide;     //��ֱ�����ƽ��
};

