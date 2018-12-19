#pragma once
#include "2d/CCSprite.h"
#include "enity/Vehicle.h"
#include "common/2D/Vector2D.h"
#include "GameWorld.h"
using namespace cocos2d;
class VehicleSprite :public Sprite
{
public:
	VehicleSprite(Vector2D pos,GameWorld *gameWorld);
	~VehicleSprite();
	static VehicleSprite* create(const std::string& file, Vector2D pos, GameWorld *gameWorld);

	bool initWithFile(const std::string& file);

	void updateS(float dt);
public:
	Vehicle *m_pVehicle;
};

