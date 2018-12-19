#include "VehicleSprite.h"
#include "2d/CCDrawNode.h"
#include "ParamLoader.h"
#define DrawTest 1;
#define DrawBorder 0;

VehicleSprite* VehicleSprite::create(const std::string& filename, Vector2D pos)
{
	VehicleSprite *sprite = new (std::nothrow) VehicleSprite(pos);
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


	Vector2D pos = m_pVehicle->Pos();
	Vector2D velocity = m_pVehicle->Velocity();
	float angle = 0;
	if (!velocity.isZero() && velocity.x!=0)
	{
		angle = atan(velocity.y / velocity.x) * 180 / pi;
	}
	float L = 30;

	//以中心点为原点
	Vec2 p1(-L/2, -L/2*tan(pi / 6));
	Vec2 p2(L/2, -L/2*tan(pi / 6));
	Vec2 p3(0, L*sin(pi / 3) - L / 2 * tan(pi / 6));
	Color4F color(1, 0, 0, 1);
	auto drawNode = DrawNode::create();
	this->addChild(drawNode, 1);
	drawNode->setPosition(pos.x,pos.y);
	Color4F color1(1, 1, 0, 1);
	drawNode->drawLine(p1, p2, color);
	drawNode->drawLine(p1, p3, color);
	drawNode->drawLine(p3, p2, color);

	drawNode->drawPoint(Vec2(0,0), 2, color1);

	drawNode->drawLine(p3, Vec2(p3.x,p3.y+5), color1);
	drawNode->setRotation(angle); //以中心点顺时针旋转

#if DrawBorder
	auto drawNodeBorder = DrawNode::create();
	this->addChild(drawNodeBorder, 1);
	drawNodeBorder->setPosition(pos.x, pos.y);
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

VehicleSprite::VehicleSprite(Vector2D pos) :
m_pVehicle(nullptr)
{
	
	m_pVehicle = new Vehicle(nullptr,
		pos,                 //initial position
		RandFloat()*TwoPi,        //start rotation
		Vector2D(0, 0),            //velocity
		Prm.VehicleMass,          //mass
		Prm.MaxSteeringForce,     //max force
		Prm.MaxSpeed,             //max velocity
		Prm.MaxTurnRatePerSecond, //max turn rate
		Prm.VehicleScale);        //scale
}


VehicleSprite::~VehicleSprite()
{
	if (m_pVehicle)
	{
		delete m_pVehicle;
		m_pVehicle = nullptr;
	}
}
