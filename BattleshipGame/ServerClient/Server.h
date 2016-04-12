#pragma once

#include "GridManager.h"
#include "InputManager.h"
#include "AIManager.h"
#include <SFML/Network.hpp>

class GridManager;
class AIManager;

class Server
{
public:
	Server() = default;

	void start();
	int launchServer();
	int sendWelcomeMessage(sf::TcpSocket* client);
	void printConnectedClients();
	void handleInput(std::vector<sf::TcpSocket*>& _sockets);
	void checkHit(int _array[11][11], int _xShot, int _yShot,
		std::vector<sf::TcpSocket*>& _sockets);

	std::vector<sf::TcpSocket*> sockets;

	int playerGrid[11][11];
	int player2Grid[11][11];

	int getHeight() { return m_gridHeight; }
	int getWidth() { return m_gridWidth; }

	int getXShot() { return m_xShot; }
	void setXShot(int _x) { m_xShot = _x; }

	int getYShot() { return m_yShot; }
	void setYShot(int _y) { m_yShot = _y; }

	bool getTurn() { return m_playerTurn; }
	void setTurn(bool _turn) { m_playerTurn = _turn; }

	GridManager* m_gridManager;
	AIManager* m_aiManager;

	AircraftCarrier* m_carrier = new AircraftCarrier();
	Battleship* m_battleship = new Battleship();
	Submarine* m_submarine = new Submarine();
	Destroyer* m_destroyer = new Destroyer();
	PatrolBoat* m_patrol = new PatrolBoat();

	AircraftCarrier* m_2Carrier = new AircraftCarrier();
	Battleship* m_2Battleship = new Battleship();
	Submarine* m_2Submarine = new Submarine();
	Destroyer* m_2Destroyer = new Destroyer();
	PatrolBoat* m_2Patrol = new PatrolBoat();
private:
	int m_gridHeight = 11;
	int m_gridWidth = 11;

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
};