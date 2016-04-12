#pragma once

class Ship 
{
public:
	Ship() = default;
	~Ship() = default;

	int GetSize(){ return m_size; }
	int GetPosX(){ return m_posX; }
	int GetPosY(){ return m_posY; }
	char GetDirection() { return m_direction; }
	bool GetPlaced() { return m_placed; }

	void SetSize(int _size) { m_size = _size; }
	void SetPosX(int _x) { m_posX = _x; }
	void SetPosY(int _y) { m_posY = _y; }
	void SetDirection(char _direction) { m_direction = _direction; }
	void SetPlaced(bool _placed) { m_placed = _placed; }

	void SetShipSize(int _size)  { m_size = _size;};

	int m_size;
	int m_posX;
	int m_posY;
	int m_health;

	char m_direction;

	bool m_placed = false;
};