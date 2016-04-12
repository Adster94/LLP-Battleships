#include "GridManager.h"
#include <iostream>
 
void GridManager::resetGrid(int _playerArray[11][11], int _player2Array[11][11], int _width, int _height)
{
	for (int r = 0; r < _width; r++)
	{
		for (int c = 0; c < _height; c++)
		{
			if (c != 0 && r == 0)
			{
				_playerArray[c][r] = c - 1;
				_player2Array[c][r] = c - 1;
			}
			else if (r != 0 && c == 0)
			{
				_playerArray[c][r] = r - 1;
				_player2Array[c][r] = r - 1;
			}
			else if (r == 0 && c == 0)
			{
				_playerArray[c][r] = 32;
				_player2Array[c][r] = 32;
			}
			else
			{
				_playerArray[c][r] = 45;
				_player2Array[c][r] = 45;
			}
		}
	}
}

void GridManager::printGrid(int _array[11][11], int _width, int _height)
{
	int letterValue = 65;
	bool hideS = false;

	/*if (_array == player2Grid)
	{
			hideS = true;
		std::cout << "Enemy Grid" << std::endl;
	}
	else
	{
		std::cout << "Player Grid" << std::endl;
	}*/

	for (int r = 0; r < _width; r++)
	{
		for (int c = 0; c < _height; c++)
		{
			if (_array[c][r] == 45)
			{
				char temp = 45;
				std::cout << temp;
			}
			else if (_array[c][r] == 32)
			{
				char temp = 32;
				std::cout << temp;
			}
			else if (_array[c][r] == 83)
			{
				if (!hideS)
				{
					char temp = 83;
					std::cout << temp;
				}
				else
				{
					char temp = 45;
					std::cout << temp;
				}
			}
			else if (_array[c][r] == 88)
			{
				char temp = 88;
				std::cout << temp;
			}
			else if (_array[c][r] == 79)
			{
				char temp = 79;
				std::cout << temp;
			}
			else if (c != 0 && r == 0)
			{
				char temp = letterValue;
				letterValue++;
				std::cout << temp;
			}
			else
			{
				std::cout << _array[c][r];
			}
			std::cout << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void GridManager::placeShip(int _array[11][11], int& _shipSize, int _shipX, int _shipY, char _direction, int& _placed,
	bool _AI, AircraftCarrier* _carrier, Battleship* _battleship, Submarine* _submarine, 
	Destroyer* _destroyer, PatrolBoat* _patrol)
{
	if (checkGrid(_array, _direction, _shipSize, _shipX, _shipY, _AI))
	{
		switch (_direction)
		{
		case 'N':
			for (int i = 0; i < _shipSize; i++)
			{
				_array[_shipX][_shipY + i] = 83;
			}
			break;
		case 'S':
			for (int i = 0; i < _shipSize; i++)
			{
				_array[_shipX][_shipY - i] = 83;
			}
			break;
		case 'E':
			for (int i = 0; i < _shipSize; i++)
			{
				_array[_shipX - i][_shipY] = 83;
			}
			break;
		case 'W':
			for (int i = 0; i < _shipSize; i++)
			{
				_array[_shipX + i][_shipY] = 83;
			}
			break;
		}
	}
	else
	{
		std::cout << "Error: Ship placement will cause collision." << std::endl
			<< "Please enter new co-ordinates." << std::endl;

		_placed -= 1;

		checkShip(_shipSize, false, _shipSize, _carrier, _battleship, _submarine, _destroyer, _patrol);
	}
}

bool GridManager::checkGrid(int _array[11][11], char _direction, int _shipSize, 
	int _shipX, int _shipY, bool _AI)
{
	for (int i = 0; i < _shipSize; i++)
	{
		switch (_direction)
		{
		case 'N':
			if (_array[_shipX][_shipY + i] == 83)
			{
				return false;
			}

			if (_AI)
			{
				if (_array[_shipX][_shipY - 1] == 83)
				{
					return false;
				}
				else if (_array[_shipX][_shipY + _shipSize] == 83)
				{
					return false;
				}
				else if (_array[_shipX - 1][_shipY + i] == 83)
				{
					return false;
				}
				else if (_array[_shipX + 1][_shipY + i] == 83)
				{
					return false;
				}
			}

			if (_shipY + i >= 11)
			{
				return false;
			}
			break;
		case 'S':
			if (_array[_shipX][_shipY - i] == 83)
			{
				return false;
			}

			if (_AI)
			{
				if (_array[_shipX][_shipY + 1] == 83)
				{
					return false;
				}
				else if (_array[_shipX][_shipY - _shipSize] == 83)
				{
					return false;
				}
				else if (_array[_shipX - 1][_shipY - i] == 83)
				{
					return false;
				}
				else if (_array[_shipX + 1][_shipY - i] == 83)
				{
					return false;
				}
			}

			if (_shipY - i < 1)
			{
				return false;
			}
			break;
		case 'E':
			if (_array[_shipX - i][_shipY] == 83)
			{
				return false;
			}

			if (_AI)
			{
				if (_array[_shipX + 1][_shipY] == 83)
				{
					return false;
				}
				else if (_array[_shipX - _shipSize][_shipY] == 83)
				{
					return false;
				}
				else if (_array[_shipX - i][_shipY - 1] == 83)
				{
					return false;
				}
				else if (_array[_shipX - i][_shipY + 1] == 83)
				{
					return false;
				}
			}

			if (_shipX - i < 1)
			{
				return false;
			}
			break;
		case 'W':
			if (_array[_shipX + i][_shipY] == 83)
			{
				return false;
			}

			if (_AI)
			{
				if (_array[_shipX - 1][_shipY] == 83)
				{
					return false;
				}
				else if (_array[_shipX + _shipSize][_shipY] == 83)
				{
					return false;
				}
				else if (_array[_shipX + i][_shipY - 1] == 83)
				{
					return false;
				}
				else if (_array[_shipX + i][_shipY + 1] == 83)
				{
					return false;
				}
			}

			if (_shipX + i >= 11)
			{
				return false;
			}
			break;
		}
	}

	return true;
}

bool GridManager::checkShip(int _number, bool _set, int& _size,
	AircraftCarrier* _carrier, Battleship* _battleship, Submarine* _submarine, 
	Destroyer* _destroyer, PatrolBoat* _patrol)
{
	switch (_number)
	{
	case 1:
		if (!_patrol->GetPlaced())
		{
			_size = _patrol->GetSize();
			_patrol->SetPlaced(true);
			return true;
		}
		else
		{
			if (!_set)
			{
				_patrol->SetPlaced(_set);
			}
			else
			{
				return false;
			}
		}
		break;
	case 2:
		if (!_destroyer->GetPlaced())
		{
			_size = _destroyer->GetSize();
			_destroyer->SetPlaced(true);
			return true;
		}
		else
		{
			if (!_set)
			{
				_destroyer->SetPlaced(_set);
			}
			else
			{
				return false;
			}
		}
		break;
	case 3:
		if (!_submarine->GetPlaced())
		{
			_size = _submarine->GetSize();
			_submarine->SetPlaced(true);
			return true;
		}
		else
		{
			if (!_set)
			{
				_submarine->SetPlaced(_set);
			}
			else
			{
				return false;
			}
		}
		break;
	case 4:
		if (!_battleship->GetPlaced())
		{
			_size = _battleship->GetSize();
			_battleship->SetPlaced(true);
			return true;
		}
		else
		{
			if (!_set)
			{
				_battleship->SetPlaced(_set);
			}
			else
			{
				return false;
			}
		}
		break;
	case 5:
		if (!_carrier->GetPlaced())
		{
			_size = _carrier->GetSize();
			_carrier->SetPlaced(true);
			return true;
		}
		else
		{
			if (!_set)
			{
				_carrier->SetPlaced(_set);
			}
			else
			{
				return false;
			}
		}
		break;
	default:
		return false;
		break;
	}
}