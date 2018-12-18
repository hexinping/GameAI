#include "VehicleSprite.h"


VehicleSprite::VehicleSprite()
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
