#pragma once
#include "2d/CCSprite.h"
#include "enity/Vehicle.h"
using namespace cocos2d;
class VehicleSprite :public Sprite
{
public:
	VehicleSprite();
	~VehicleSprite();
	static VehicleSprite* create(const std::string& file);

	bool initWithFile(const std::string& file);
public:
	Vehicle *m_pVehicle;
};

