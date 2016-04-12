#pragma once

#include "AircraftCarrier.h"
#include "Battleship.h"
#include "Submarine.h"
#include "Destroyer.h"
#include "PatrolBoat.h"

class GridManager
{
public:
	void resetGrid(int _playerArray[11][11], int _player2Array[11][11], int _width, int _height);
	void printGrid(int _array[11][11], int _width, int _height);
	void placeShip(int _array[11][11], int& _shipSize, int _shipX, int _shipY, char _direction, int& _placed, 
		bool _AI, AircraftCarrier* _carrier, Battleship* _battleship, Submarine* _submarine, Destroyer* _destroyer, PatrolBoat* _patrol);
	bool checkGrid(int _array[11][11], char _direction, int _shipSize, int _shipX, int _shipY, bool _AI);
	bool checkShip(int _number, bool _set, int& _size,
		AircraftCarrier* _carrier, Battleship* _battleship, Submarine* _submarine, 
		Destroyer* _destroyer, PatrolBoat* _patrol);
};