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
	_pMinerWife = new MinerWife(ent_Elsa);

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
		_pMinerWife->Update();
	}
}

MineScene::~MineScene()
{
	if (_pMiner)
	{
		delete _pMiner;
		_pMiner = nullptr;
	}

	if (_pMinerWife)
	{
		delete _pMinerWife;
		_pMinerWife = nullptr;
	}
	
}