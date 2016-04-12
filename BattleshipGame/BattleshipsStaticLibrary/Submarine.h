#pragma once

#include "Ship.h"

class Submarine : public Ship
{
public:
	Submarine()
	{
		m_size = 3;
		m_health = m_size;
	}
	~Submarine() = default;

private:

};