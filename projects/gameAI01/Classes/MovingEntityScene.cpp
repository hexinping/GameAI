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

	m_pVehicleSprite = VehicleSprite::create("", startPos, targetPosArrive, m_pGameWorld);

	this->addChild(m_pVehicleSprite);

	auto drawNode = DrawNode::create();
	this->addChild(drawNode, 1);
	Color4F color1(1, 1, 0, 1);

	EState state = EState::Arrive;
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

		m_pVehicleSprite->updateS(dt);
		Dispatch->DispatchDelayedMessages();
	}
}

MovingEntityScene::~MovingEntityScene()
{
	this->unscheduleUpdate();
	if (m_pGameWorld)
	{
		delete m_pGameWorld;
		m_pGameWorld = nullptr;
	}
}