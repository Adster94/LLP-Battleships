#pragma once

#include "Ship.h"

class AircraftCarrier : public Ship
{
public:
	AircraftCarrier() 
	{ 
		m_size = 5;
		m_health = m_size;
	}
	~AircraftCarrier() = default;

private:

};