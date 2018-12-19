#include "VehicleSprite.h"
#include "2d/CCDrawNode.h"
#include "ParamLoader.h"
#define DrawTest 1;
#define DrawBorder 0;

VehicleSprite* VehicleSprite::create(const std::string& filename, Vector2D pos,GameWorld *gameWorld)
{
	VehicleSprite *sprite = new (std::nothrow) VehicleSprite(pos, gameWorld);
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
	Vector2D pos = m_pVehicle->Pos();
	this->setPosition(pos.x, pos.y);
#if DrawTest

	Vector2D velocity = m_pVehicle->Velocity();
	const float EPSINON = 0.00001;
	
	bool isX0 = false;
	if ((velocity.x >= -EPSINON) && (velocity.x <= EPSINON))
	{
		isX0 = true;
	}

	bool isY0 = false;
	if ((velocity.y >= -EPSINON) && (velocity.y <= EPSINON))
	{
		isY0 = true;
	}
	float angle = 0;
	if (!velocity.isZero() && !isX0)
	{
		
		if (isY0)
		{
			angle = 90;
		}
		else
		{
			angle = atan(velocity.y / velocity.x) * 180 / pi;
		}
	}
	float L = 30;

	//以中心点为原点
	Vec2 p1(-L/2, -L/2*tan(pi / 6));
	Vec2 p2(L/2, -L/2*tan(pi / 6));
	Vec2 p3(0, L*sin(pi / 3) - L / 2 * tan(pi / 6));
	Color4F color(1, 0, 0, 1);
	auto drawNode = DrawNode::create();
	this->addChild(drawNode, 1);
	
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

VehicleSprite::VehicleSprite(Vector2D pos, GameWorld *gameWorld) :
m_pVehicle(nullptr)
{

	m_pVehicle = new Vehicle(gameWorld,
		pos,                 //initial position
		RandFloat()*TwoPi,        //start rotation
		Vector2D(10, 0),            //velocity
		Prm.VehicleMass,          //mass
		Prm.MaxSteeringForce,     //max force
		Prm.MaxSpeed,             //max velocity
		Prm.MaxTurnRatePerSecond, //max turn rate
		Prm.VehicleScale);        //scale

	
}


void VehicleSprite::updateS(float dt)
{
	if (m_pVehicle)
	{
		m_pVehicle->Update(dt);
		Vector2D pos = m_pVehicle->Pos();
		this->setPosition(pos.x, pos.y);

	}
}

VehicleSprite::~VehicleSprite()
{
	if (m_pVehicle)
	{
		delete m_pVehicle;
		m_pVehicle = nullptr;
	}
}
