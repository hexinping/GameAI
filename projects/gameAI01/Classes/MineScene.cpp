#include "MineScene.h"
#include "enity/Miner.h"

USING_NS_CC;

enum
{
	ent_Miner_Bob,

	ent_Elsa
};


Scene* MineScene::createScene()
{
	return MineScene::create();
}

// on "init" you need to initialize your instance
bool MineScene::init()
{
    
	_pMiner = new Miner(ent_Miner_Bob);


	this->scheduleUpdate();

    return true;
}


void MineScene::update(float dt)
{
	//CCLOG("baibai");
	sleepTime = sleepTime + dt;
	if (sleepTime>=1.0)
	{
		sleepTime = 0;
		_pMiner->Update();
	}
}