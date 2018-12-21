#include "VehicleSprite.h"
#include "2d/CCDrawNode.h"
#include "ParamLoader.h"
#include "behaviors/SteeringBehaviors.h"
#define DrawTest 1;
#define DrawBorder 0;

VehicleSprite* VehicleSprite::create(const std::string& filename, Vector2D pos, Vector2D targetPos, GameWorld *gameWorld)
{
	VehicleSprite *sprite = new (std::nothrow) VehicleSprite(pos, targetPos, gameWorld);
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
	Vector2D pos = m_vPos;
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

VehicleSprite::VehicleSprite(Vector2D pos, Vector2D targetPos, GameWorld *gameWorld) :
m_pVehicle(nullptr),
m_vVelocity(Vector2D(1, 0)),
m_vPos(pos),
m_startPos(pos),
m_targetPos(targetPos),
m_vHeading(Vector2D(0, 0)),
m_vSide(Vector2D(0, 0)),
m_state(EState::None)
{

	m_pVehicle = new Vehicle(gameWorld,
		pos,                 //initial position
		0,        //start rotation
		m_vVelocity,            //velocity
		Prm.VehicleMass,          //mass
		Prm.MaxSteeringForce,     //max force
		Prm.MaxSpeed,             //max velocity
		Prm.MaxTurnRatePerSecond, //max turn rate
		Prm.VehicleScale);        //scale

	m_pVehicle->Update(0);
	//m_pVehicle->World()->SetCrosshair(Vector2D(pos.x + 100, pos.y + 200));
	m_pVehicle->Steering()->SeekOn();
	//m_pVehicle->Steering()->ArriveOn();

	//设置状态，不同状态获得不同的效果
	//m_state = EState::Seek;
	m_state = EState::Flee;
	//m_targetPos = Vector2D(pos.x + 200, pos.y + 200);

}


void VehicleSprite::updateS(float dt)
{
	if (m_pVehicle)
	{
		
		//Vector2D pos = m_pVehicle->Pos();
		//Vector2D targetPos = m_pVehicle->World()->Crosshair();
		//CCLOG("posx posy targetposx targetposy: %f, %f, %f, %f", pos.x, pos.y, targetPos.x, targetPos.y);
		//if (pos.x >= targetPos.x)
		//{
		//	m_pVehicle->SetVelocity(Vector2D(0, 0));
		//	return;
		//}
		//m_pVehicle->Update(dt);
		//Vector2D head = m_pVehicle->Heading();
		//this->setPosition(pos.x, pos.y);
		//this->setHead(head);






		///////////////////////////////////////////////
		if (m_state ==EState::Seek && isSeekOver())
		{
			return;
		}

		//记录上一次更新的速度
		Vector2D OldPos = this->getPos();


		//计算操作力
		Vector2D SteeringForce;
		SteeringForce = this->calculate();

		//计算加速度
		Vector2D acceleration = SteeringForce / m_dMass;

		//更新速度
		m_vVelocity += acceleration * dt;

		//确保不超过最大速度
		m_vVelocity.Truncate(m_dMaxSpeed);

		//更新位置
		m_vPos += m_vVelocity * dt;

		//update the heading if the vehicle has a non zero velocity
		if (m_vVelocity.LengthSq() > 0.00000001)
		{
			//因为交通工具的朝向总是跟速度一致，所以需要更新，使其等于速度的标准向量
			m_vHeading = Vec2DNormalize(m_vVelocity);

			//计算出垂直于朝向的平面
			m_vSide = m_vHeading.Perp();
		}

		//EnforceNonPenetrationConstraint(this, World()->Agents());

		////边界判断  todo
		//WrapAround(m_vPos, m_pWorld->cxClient(), m_pWorld->cyClient());

		////update the vehicle's current cell if space partitioning is turned on
		//if (Steering()->isSpacePartitioningOn())
		//{
		//	World()->CellSpace()->UpdateEntity(this, OldPos);
		//}

		//if (isSmoothingOn())
		//{
		//	m_vSmoothedHeading = m_pHeadingSmoother->Update(Heading());
		//}


		this->setPosition(m_vPos.x, m_vPos.y);
		this->setHead(m_vHeading);

	}
}

bool VehicleSprite::isSeekOver()
{
	bool isOverSeek = false;
	if (m_targetPos.x > m_startPos.x)
	{
		if (m_vPos.x >= m_targetPos.x)
		{
			isOverSeek = true;
		}
	}
	else
	{
		if (m_vPos.x <= m_targetPos.x)
		{
			isOverSeek = true;
		}
	}

	return isOverSeek;
}
//返回seek的力
Vector2D VehicleSprite::seekForce(Vector2D TargetPos)
{
	Vector2D DesiredVelocity = Vec2DNormalize(TargetPos - this->getPos())
		* m_dMaxSpeed;

	return (DesiredVelocity - m_vVelocity);
}

//返回flee的力
Vector2D VehicleSprite::fleeForce(Vector2D TargetPos)
{
	//only flee if the target is within 'panic distance'. Work in distance
	//squared space.
	 const double PanicDistanceSq = 100.0f * 100.0;
	 //在目标点一定区域内才产生离开的力
	 if (Vec2DDistanceSq(this->getPos(), TargetPos) > PanicDistanceSq)
	 {
		 return Vector2D(0, 0);
	 }
	

	Vector2D DesiredVelocity = Vec2DNormalize(this->getPos() - TargetPos)
		* m_dMaxSpeed;

	return (DesiredVelocity - m_vVelocity);
}

//根据不同状态计算合力
Vector2D VehicleSprite::calculate()
{

	if (m_state == EState::Seek)
	{
		return seekForce(m_targetPos);
	}
	else if (m_state == EState::Flee)
	{
		return fleeForce(m_targetPos);
	}

	return Vector2D(0, 0);

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
