#include "MovingEntityScene.h"
#include "common/messaging/MessageDispatcher.h"
#include "enity/MovingEntity.h"
#include "enity/Vehicle.h"
#include "VehicleSprite.h"

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

	auto s = VehicleSprite::create("HelloWorld.png");

	this->addChild(s);
	s->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

	this->scheduleUpdate();

    return true;
}


void MovingEntityScene::update(float dt)
{
	//CCLOG("baibai");
	sleepTime = sleepTime + dt;
	if (sleepTime>=1.0)
	{
		sleepTime = 0;
		

		Dispatch->DispatchDelayedMessages();
	}
}

MovingEntityScene::~MovingEntityScene()
{

	
}