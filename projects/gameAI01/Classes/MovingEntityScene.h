#ifndef __MovingEntityScene_SCENE_H__
#define __MovingEntityScene_SCENE_H__

#include "cocos2d.h"

class MovingEntityScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

	~MovingEntityScene();

	void update(float dt);
    
    // implement the "static create()" method manually
	CREATE_FUNC(MovingEntityScene);

public:
	
	float sleepTime = 0.0;
};

#endif // __HELLOWORLD_SCENE_H__
