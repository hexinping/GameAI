#pragma once
#include "2d/CCSprite.h"
#include "enity/Vehicle.h"
using namespace cocos2d;
class VehicleSprite :public Sprite
{
public:
	VehicleSprite();
	~VehicleSprite();

public:
	Vehicle *m_pVehicle;
};

