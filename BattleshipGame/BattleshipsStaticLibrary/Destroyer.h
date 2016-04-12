#pragma once

#include "Ship.h"

class Destroyer : public Ship
{
public:
	Destroyer()
	{
		m_size = 2;
		m_health = m_size;
	}
	~Destroyer() = default;

private:

};