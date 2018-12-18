#include "MovingEntityScene.h"
#include "common/messaging/MessageDispatcher.h"
#include "enity/MovingEntity.h"
#include "enity/Vehicle.h"

USING_NS_CC;


Scene* MovingEntityScene::createScene()
{
	return MovingEntityScene::create();
}

// on "init" you need to initialize your instance
bool MovingEntityScene::init()
{
    
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