#include "VehicleSprite.h"
#include "2d/CCDrawNode.h"

#define DrawTest 1;
#define DrawBorder 0;

VehicleSprite* VehicleSprite::create(const std::string& filename)
{
	VehicleSprite *sprite = new (std::nothrow) VehicleSprite();
	if (sprite && sprite->initWithFile(filename))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}


bool VehicleSprite::initWithFile(const std::string& file)
{
	Sprite::initWithFile(file);

#if DrawTest

	float L = 30;
	Vec2 p1(0, 0);
	Vec2 p2(L, 0);
	Vec2 p3(L / 2.0, L*sin(pi / 3));
	Color4F color(1, 0, 0, 1);
	auto drawNode = DrawNode::create();
	this->addChild(drawNode, 1);
	drawNode->setPosition(-L / 2, -L / 2);

	drawNode->drawLine(p1, p2, color);
	drawNode->drawLine(p1, p3, color);
	drawNode->drawLine(p3, p2, color);


#if DrawBorder
	auto drawNodeBorder = DrawNode::create();
	this->addChild(drawNodeBorder, 1);
	float width = L;
	float height = L;
	drawNodeBorder->drawLine(Vec2(-width / 2, -height / 2), Vec2(-width / 2, height / 2), color);
	drawNodeBorder->drawLine(Vec2(-width / 2, height / 2), Vec2(width / 2, height / 2), color);
	drawNodeBorder->drawLine(Vec2(width / 2, height / 2), Vec2(width / 2, -height / 2), color);
	drawNodeBorder->drawLine(Vec2(width / 2, -height / 2), Vec2(-width / 2, -height / 2), color);
#endif
	
	
#endif
	return true;
}

VehicleSprite::VehicleSprite():
m_pVehicle(nullptr)
{

	
}


VehicleSprite::~VehicleSprite()
{
	if (m_pVehicle)
	{
		delete m_pVehicle;
		m_pVehicle = nullptr;
	}
}
