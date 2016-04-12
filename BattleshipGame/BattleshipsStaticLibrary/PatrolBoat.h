#pragma once

#include "Ship.h"

class PatrolBoat : public Ship
{
public:
	PatrolBoat()
	{
		m_size = 1;
		m_health = m_size;
	}
	~PatrolBoat() = default;

private:

};