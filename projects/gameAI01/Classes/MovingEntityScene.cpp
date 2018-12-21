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

	m_pVehicleSprite = VehicleSprite::create("", Vector2D(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y), m_pGameWorld);

	this->addChild(m_pVehicleSprite);

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