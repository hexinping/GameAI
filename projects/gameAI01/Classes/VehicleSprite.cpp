#include "VehicleSprite.h"
#include "2d/CCDrawNode.h"
#include "ParamLoader.h"
#include "behaviors/SteeringBehaviors.h"
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

	float L = 60;
	Vec2 center(0,0);
	float h = L/2 * tan(pi/6);
	float H = L * sin(pi/3);
	Vec2 p1 = Vec2(-h,L/2);
	Vec2 p2 = Vec2(-h,-L/2);
	Vec2 p3 = Vec2(H-h,0);
	Color4F color(1, 0, 0, 1);
	auto drawNode = DrawNode::create();
	this->addChild(drawNode, 1);
	drawNode->drawLine(p1, p2, color);
	drawNode->drawLine(p1, p3, color);
	drawNode->drawLine(p3, p2, color);

	Color4F color1(1, 1, 0, 1);
	drawNode->drawPoint(center, 2, color1);
	drawNode->drawLine(p3, Vec2(p3.x + 5, p3.y), color1);
	this->setHead(m_pVehicle->Heading());
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
		0,        //start rotation
		Vector2D(1, 0),            //velocity
		Prm.VehicleMass,          //mass
		Prm.MaxSteeringForce,     //max force
		Prm.MaxSpeed,             //max velocity
		Prm.MaxTurnRatePerSecond, //max turn rate
		Prm.VehicleScale);        //scale

	m_pVehicle->Update(0);
	m_pVehicle->World()->SetCrosshair(Vector2D(512 + 100, 364 + 200));
	m_pVehicle->Steering()->SeekOn();
	//m_pVehicle->Steering()->ArriveOn();
}


void VehicleSprite::updateS(float dt)
{
	if (m_pVehicle)
	{
		
		Vector2D pos = m_pVehicle->Pos();
		Vector2D targetPos = m_pVehicle->World()->Crosshair();
		CCLOG("posx posy targetposx targetposy: %f, %f, %f, %f", pos.x, pos.y, targetPos.x, targetPos.y);
		if (pos.x >= targetPos.x)
		{
			m_pVehicle->SetVelocity(Vector2D(0, 0));
			return;
		}
		m_pVehicle->Update(dt);
		Vector2D head = m_pVehicle->Heading();
		this->setPosition(pos.x, pos.y);
		this->setHead(head);

	}
}

void VehicleSprite::setHead(Vector2D head)
{
	const float EPSINON = 0.00001;

	bool isX0 = false;
	if ((head.x >= -EPSINON) && (head.x <= EPSINON))
	{
		isX0 = true;
	}

	bool isY0 = false;
	if ((head.y >= -EPSINON) && (head.y <= EPSINON))
	{
		isY0 = true;
	}
	float angle = 0;

	if (!isX0 && !isY0)
	{
		float a = head.y / head.x;
		angle = atan(a) * 180 / pi;
		if (a<0)
		{
			if (head.x<0)
			{
				angle = 180 - angle;
			}
			else
			{
				angle = -angle;
			}
		}
		else
		{
			if (head.x<0)
			{
				angle = -180-angle;
			}
			else
			{
				angle = -angle;
			}
			
		}
	}
	else if (isX0 && !isY0)
	{
		if (head.y > 0)
		{
			angle = -90;
		}
		else
		{
			angle = 90;
		}
		
	}
	else if (isY0)
	{
		if (head.x>0)
		{
			angle = 0;
		}
		else
		{
			angle = -180;
		}
		
	}
	this->setRotation(angle);
}
VehicleSprite::~VehicleSprite()
{
	if (m_pVehicle)
	{
		delete m_pVehicle;
		m_pVehicle = nullptr;
	}

}
