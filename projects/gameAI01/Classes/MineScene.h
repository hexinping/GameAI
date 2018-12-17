#ifndef __MineScene_SCENE_H__
#define __MineScene_SCENE_H__

#include "cocos2d.h"
#include "enity/Miner.h"
#include "enity/MinerWife.h"

class MineScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

	~MineScene();

	void update(float dt);
    
    // implement the "static create()" method manually
	CREATE_FUNC(MineScene);

public:
	Miner * _pMiner;
	MinerWife *_pMinerWife;
	float sleepTime = 0.0;
};

#endif // __HELLOWORLD_SCENE_H__
