#include "VehicleSprite.h"
#include "2d/CCDrawNode.h"
#include "ParamLoader.h"
#include "behaviors/SteeringBehaviors.h"
#include <math.h>
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
m_vVelocity(Vector2D(0, 0)),
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
	//m_state = EState::Arrive;
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



		if (m_state == EState::None)
		{
			return;
		}


		///////////////////////////////////////////////
		if (m_state ==EState::Seek && isSeekOver())
		{
			m_vVelocity.Zero();
			return;
		}

		if (m_state == EState::Arrive && isArriveOver())
		{
			m_vVelocity.Zero();
			return;
		}

		if (m_state == EState::Pursuit && isPursuitOver())
		{
			m_vVelocity.Zero();
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

	Vector2D force = DesiredVelocity - m_vVelocity;
	this->accumulateForce(m_vSteeringForce, force);
	return m_vSteeringForce;
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
	Vector2D force = DesiredVelocity - m_vVelocity;
	this->accumulateForce(m_vSteeringForce, force);
	return m_vSteeringForce;
}


Vector2D VehicleSprite::arriveForce(Vector2D TargetPos, Deceleration deceleration)
{

	Vector2D ToTarget = TargetPos - this->getPos();

	//计算当前位置离目标的距离
	double dist = ToTarget.Length();

	if (dist > 0)
	{
		
		//因为Deceleration是整数，所以需要这个值提供调整减速度
		const double DecelerationTweaker = 3.0;

		
		//给定预期减速度，计算能达到目标位置需要速度
		double speed = dist / ((double)deceleration * DecelerationTweaker);

		//make sure the velocity does not exceed the max
		speed = fmin(speed, m_dMaxSpeed);

		//from here proceed just like Seek except we don't need to normalize 
		//the ToTarget vector because we have already gone to the trouble
		//of calculating its length: dist. 
		Vector2D DesiredVelocity = ToTarget * speed / dist;


		Vector2D force = DesiredVelocity - m_vVelocity;
		this->accumulateForce(m_vSteeringForce,force);
		return m_vSteeringForce;
	}

	return Vector2D(0, 0);
}

bool VehicleSprite::isArriveOver()
{
	bool isOverArray = false;

	if (abs(m_targetPos.x - m_vPos.x) <= 1.0 && abs(m_targetPos.y - m_vPos.y) <= 1.0)
	{
		isOverArray = true;
	}
	return isOverArray;
}

Vector2D VehicleSprite::pursuitForce( VehicleSprite* evader)
{
	//预测逃避目标的移动点

	//如果逃避者在前面而且面对者追逐者，直接靠近逃避者的当前位置
	Vector2D ToEvader = evader->getPos() - this->getPos();

	double RelativeHeading = m_vHeading.Dot(evader->m_vHeading);

	//如果逃避者在前方。并且相对朝向在20度之内 就直接靠近逃避者位置
	if ((ToEvader.Dot(m_vHeading) > 0) &&
		(RelativeHeading < -0.95))  //acos(0.95)=18 degs
	{
		return seekForce(evader->getPos());
	}


	//预测逃避者的位置
	//预测的时间正比于两者的距离，反比于两者的速度大小
	double LookAheadTime = ToEvader.Length() /
		(m_dMaxSpeed + evader->m_vVelocity.Length());

	//靠近逃避者被预测的位置
	return seekForce(evader->getPos() + evader->m_vVelocity * LookAheadTime);
}

bool VehicleSprite::isPursuitOver()
{
	bool isOver = false;
	Vector2D pursuitPos = m_pursuitTarget->getPos();
	CCLOG("posx posy pursuitposx pursuitposy==%f,%f,%f,%f",m_vPos.x,m_vPos.y,pursuitPos.x,pursuitPos.y);

	float dis = m_vPos.DistanceSq(pursuitPos);

	if (abs(pursuitPos.x - m_vPos.x) <=5.0  && abs(pursuitPos.y - m_vPos.y) <=5.0)
	{
		isOver = true;
	}

	return isOver;
}



//根据不同状态计算合力
Vector2D VehicleSprite::calculate()
{
	m_vSteeringForce.Zero();

	//权重
	float SeekWeight = 1.0;
	float FleeWeight = 1.0;
	float ArriveWeight = 1.0;
	float PursuitWeight = 1.0;


	if (m_state == EState::Seek)
	{
		return seekForce(m_targetPos)*SeekWeight;
	}
	else if (m_state == EState::Flee)
	{
		return fleeForce(m_targetPos)*FleeWeight;
	}
	else if (m_state == EState::Arrive)
	{
		return arriveForce(m_targetPos)*ArriveWeight;
	}
	else if (m_state == EState::Pursuit)
	{
		return pursuitForce(m_pursuitTarget)*PursuitWeight;
	}
	return Vector2D(0, 0);

}


bool VehicleSprite::accumulateForce(Vector2D &RunningTot, Vector2D ForceToAdd)
{

	//calculate how much steering force the vehicle has used so far
	float MagnitudeSoFar = RunningTot.Length();

	//calculate how much steering force remains to be used by this vehicle
	float MagnitudeRemaining = m_maxSteeringForce - MagnitudeSoFar;

	//return false if there is no more force left to use
	if (MagnitudeRemaining <= 0.0) return false;

	//calculate the magnitude of the force we want to add
	float MagnitudeToAdd = ForceToAdd.Length();

	//if the magnitude of the sum of ForceToAdd and the running total
	//does not exceed the maximum force available to this vehicle, just
	//add together. Otherwise add as much of the ForceToAdd vector is
	//possible without going over the max.
	if (MagnitudeToAdd < MagnitudeRemaining)
	{
		RunningTot += ForceToAdd;
	}

	else
	{
		//add it to the steering force
		RunningTot += (Vec2DNormalize(ForceToAdd) * MagnitudeRemaining);
	}

	return true;
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
