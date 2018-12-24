#include "MovingEntityScene.h"
#include "common/messaging/MessageDispatcher.h"
#include "enity/MovingEntity.h"
#include "enity/Vehicle.h"
#include "VehicleSprite.h"
#include "common/2D/Vector2D.h"

USING_NS_CC;

Scene* MovingEntityScene::createScene()
{
	return MovingEntityScene::create();
}

// on "init" you need to initialize your instance
bool MovingEntityScene::init()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	m_pGameWorld = new GameWorld(visibleSize.width, visibleSize.height);

	Vector2D startPos = Vector2D(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);

	Vector2D targetPosSeek(startPos.x + 200, startPos.y + 200);
	Vector2D targetPosFlee(startPos.x + 40, startPos.y + 20);
	Vector2D targetPosArrive(startPos.x + 200, startPos.y + 200);
	

	m_pVehicleSprite = VehicleSprite::create("", Vector2D(startPos.x - 100, startPos.y), targetPosArrive, m_pGameWorld);

	this->addChild(m_pVehicleSprite);
	m_pVehicleSprite->setCurstate(EState::Pursuit);

	

	//////////////////////////////////////
	VehicleSprite *test = VehicleSprite::create("", startPos, Vector2D(targetPosArrive.x - 250, targetPosArrive.y + 50), m_pGameWorld);
	this->addChild(test);
	test->setCurstate(EState::Evade);

	m_pVehicleSprite->setPursuitTarget(test);
	test->setEvaderTarget(m_pVehicleSprite);
	/////////////////////////////////////


	m_vector.push_back(m_pVehicleSprite);
	m_vector.push_back(test);


	auto drawNode = DrawNode::create();
	this->addChild(drawNode, 1);
	Color4F color1(1, 1, 0, 1);

	EState state = EState::Pursuit;
	Vector2D targetPos;
	if (state == EState::Seek)
	{
		targetPos = targetPosSeek;
	}
	else if (state == EState::Flee)
	{
		targetPos = targetPosFlee;
		drawNode->drawCircle(Vec2(targetPos.x, targetPos.y), 100, 0, 30, false, color1);
	}
	else if (state == EState::Arrive)
	{
		targetPos = targetPosArrive;
	}

	drawNode->drawPoint(Vec2(targetPos.x, targetPos.y), 8, color1);


	this->scheduleUpdate();

    return true;
}


void MovingEntityScene::update(float dt)
{
	sleepTime = sleepTime + dt;
	if (sleepTime >= 0.01)
	{
		sleepTime = 0;

		for (int i = 0; i < m_vector.size();i++)
		{
			VehicleSprite *p = m_vector[i];
			p->updateS(dt);
		}

		//m_pVehicleSprite->updateS(dt);
		Dispatch->DispatchDelayedMessages();
	}
}

MovingEntityScene::~MovingEntityScene()
{
	this->unscheduleUpdate();
	m_vector.clear();
	if (m_pGameWorld)
	{
		delete m_pGameWorld;
		m_pGameWorld = nullptr;
	}
}