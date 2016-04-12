#pragma once

#include "Server.h"
#include "GridManager.h"
#include "AIManager.h"
#include <vector>
#include <SFML/Network.hpp>

class Server;
class AIManager;
class GridManager;

class InputManager
{
public:
	InputManager(GridManager* _gridManager)
	{
		m_gridManager = _gridManager;
	}

	void handleInput(std::vector<sf::TcpSocket*>& _sockets);
	void checkHit(int _array[11][11], int _xShot, int _yShot, 
		std::vector<sf::TcpSocket*>& _sockets);

	int getXShot() { return m_xShot; }
	void setXShot(int _x) { m_xShot = _x; }

	int getYShot() { return m_yShot; }
	void setYShot(int _y) { m_yShot = _y; }

	bool getTurn() { return m_playerTurn; }
	void setTurn(bool _turn) { m_playerTurn = _turn; }

private:
	int m_xShot = 0;
	int m_yShot = 0;

	bool m_playerTurn = true;

	std::string type = "T";
	std::string xShotString = "A";
	std::string yShotString = "B";
	std::string ship = "S";
	std::string direction = "D";
	std::string xLocation = "X";
	std::string yLocation = "Y";
	std::string placed = "P";

	GridManager* m_gridManager;
	AIManager* m_aiManager;
	Server* m_server;
};