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
	Arrive, //�ִ�
	Pursuit, //׷��
	Evade,   //�ӱ�
};


enum Deceleration{ slow = 3, normal = 2, fast = 1 };


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

	//���صִ����
	Vector2D arriveForce(Vector2D TargetPos,Deceleration deceleration = Deceleration::fast);
	bool isArriveOver();

	bool accumulateForce(Vector2D &RunningTot, Vector2D ForceToAdd);

	//����׷�������Ŀ��ΪԤ��λ��
	Vector2D pursuitForce( VehicleSprite* evader);
	bool isPursuitOver();

	Vector2D evadeForce(VehicleSprite* pursuer);
	bool isEvadeOver();


	void setPursuitTarget(VehicleSprite *t){ m_pursuitTarget = t; }
	void setEvaderTarget(VehicleSprite *t){ m_evaderTarget = t; }

	void setCurstate(EState s){ m_state = s; };

	float turnAroundTime(Vector2D targetPos);

	//�߽����
	void wapAround(Vector2D &pos, int MaxX, int MaxY);

public:
	Vehicle *m_pVehicle;

	EState m_state;

	//�߽����x ���y
	float m_cxClient;
	float m_cyClient;

	//�˶�ѧ��ر���
	float m_dMass = 1.0; //����
	float m_dMaxSpeed = 150; //����ٶ�
	float m_maxSteeringForce = 400; //��������

	Vector2D m_targetPos;
	Vector2D m_startPos;

	Vector2D m_vSteeringForce; //��ǰ�������

	Vector2D m_vVelocity; //��ͨ����ٶ�
	Vector2D m_vPos;      //��ͨ���λ��
	Vector2D m_vHeading;  //����
	Vector2D m_vSide;     //��ֱ�����ƽ��

	//׷��Ŀ�� 
	VehicleSprite *m_pursuitTarget;
	//�ӱ�Ŀ��
	VehicleSprite *m_evaderTarget;

};

