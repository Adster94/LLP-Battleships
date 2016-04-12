#pragma once

#include "AircraftCarrier.h"
#include "Battleship.h"
#include "Submarine.h"
#include "Destroyer.h"
#include "PatrolBoat.h"

#include "GridManager.h"
#include "Server.h"

class Client
{
public:
	void Listen();
	//void resetGrid();
	//void printGrid(int _array[11][11]);
	//void placeShip(int _array[11][11], int& _shipSize, int _shipX, int _shipY, char _direction);
	int checkLetter(char _letter);
	void checkHit(int _array[11][11], int _xShot, int _yShot);
	//bool checkGrid(int _array[11][11], char _direction, int _shipSize, int _shipX, int _shipY);
	void playerInput();
	void hitMiss(int _array[11][11], char _xShot, int _yShot);
	void initialiseEnemy();
	void startingScreen();
	void EnemyAttack(bool _attackAI);
	//void checkShip(int _number, bool _set);

	bool GetPlaying() { return m_playing; }
	void SetPlaying(bool _playing) { m_playing = _playing; }

	bool GetEnemy() { return m_enemy; }
	void SetEnemy(bool _enemy) { m_enemy = _enemy; }

	bool GetPlacement() { return m_placement; }
	void SetPlacement(bool _placement) { m_placement = _placement; }

	bool GetTurn() { return m_playerTurn; }
	void SetTurn(bool _turn) { m_playerTurn = _turn; }

	int GetShipPlaced() { return m_shipPlaced; }
	void SetShipPlaced(int _placed) { m_shipPlaced = _placed; }

	bool GetAI() { return m_AI; }
	void SetAI(bool _ai) { m_AI = _ai; }

	bool GetPlaceable() { return m_placeable; }
	void SetPlaceable(bool _placeable) { m_placeable = _placeable; }

	int GetShipSize() { return m_shipSize; }
	void SetShipSize(int _shipSize) { m_shipSize = _shipSize; }

	int playerGrid[11][11];
	int player2Grid[11][11];

protected:
	int m_gridHeight = 11;
	int m_gridWidth = 11;
	int m_shipX;
	int m_shipY;
	int m_shipSize = 1;
	char m_direction;
	char m_xShot;
	int m_yShot;
	int m_shipNumber = 0;
	int m_shipPlaced = 0;

	int m_enemyShipSize = 1;
	int m_enemyPlaced = 0;
	int m_enemyPrevX = 0;
	int m_enemyPrevY = 0;
	int m_enemyXPos = 0;
	int m_enemyYPos = 0;

	int attackCounter = 0;

	bool m_playing = true;
	bool m_placement = true;
	bool m_enemy = true;
	bool m_placeable = false;
	bool m_playerTurn = true;
	bool m_AI = false;
	bool m_singlePlayer = false;

	AircraftCarrier* m_carrier;
	Battleship* m_battleship;
	Submarine* m_submarine;
	Destroyer* m_destroyer;
	PatrolBoat* m_patrol;

	AircraftCarrier* m_eCarrier;
	Battleship* m_eBattleship;
	Submarine* m_eSubmarine;
	Destroyer* m_eDestroyer;
	PatrolBoat* m_ePatrol;

	GridManager* m_gridManager;
	Server* m_server;
};
