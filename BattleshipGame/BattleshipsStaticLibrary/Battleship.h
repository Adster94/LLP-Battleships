#pragma once

#include "Ship.h"

class Battleship : public Ship
{
public:
	Battleship()
	{
		m_size = 4;
		m_health = m_size;
	}
	~Battleship() = default;

private:

};