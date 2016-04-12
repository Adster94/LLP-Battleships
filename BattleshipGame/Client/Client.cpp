#include "stdafx.h"

#include "Client.h"
#include <iostream>
#include <SFML/Network.hpp>
#include <chrono>
#include <thread>
#include <ctime>
#include <future>
#include <iostream>

sf::TcpSocket socket;
sf::SocketSelector selector;

void Client::Listen()
{
	auto handle = std::async(std::launch::async, [] {

		while (true)
		{
			sf::Packet packet;
			if (selector.wait() && selector.isReady(socket))
			{
				if (socket.receive(packet) == sf::Socket::Done)
				{
					//convert to string
					std::string tag;
					packet >> tag;

					if (tag == "ping")
					{
						//send relpy
						sf::Packet pong;
						pong << "pong";
						socket.send(pong);
					}

					if (tag == "hit")
					{
						//update hit
						//_enemyGrid[11][11] = 88;
					}
					if (tag == "miss")
					{
						//update miss
						//_enemyGrid[11][11] = 79;
					}

					std::cout << "> " << tag << std::endl;
				}
			}
		}
	});

	while (true)
	{
		char data[2048];
		std::cin.get(data, 2048);
		std::cin.ignore();
		std::cin.clear();

		sf::Packet packet;
		packet << data;
		socket.send(packet);
	}
}

void Client::startingScreen()
{
	srand(time(NULL));

	int play;
	std::cout << "... Welcome to Battleships ..." << std::endl;
	std::cout << "Press 1 to play single-player" << std::endl;
	std::cout << "Press 2 to play multi-player" << std::endl;
	std::cin >> play;

	m_gridManager = new GridManager();
	m_server = new Server();

	switch (play)
	{
	case 1:
		m_singlePlayer = true;

		m_carrier = new AircraftCarrier();
		m_battleship = new Battleship();
		m_submarine = new Submarine();
		m_destroyer = new Destroyer();
		m_patrol = new PatrolBoat();

		m_eCarrier = new AircraftCarrier();
		m_eBattleship = new Battleship();
		m_eSubmarine = new Submarine();
		m_eDestroyer = new Destroyer();
		m_ePatrol = new PatrolBoat();

		m_gridManager->resetGrid(playerGrid, player2Grid, m_gridWidth, m_gridHeight);
		SetAI(true);
		initialiseEnemy();
		m_gridManager->printGrid(player2Grid, m_gridWidth, m_gridHeight);

		while (GetPlaying())
		{
			playerInput();
		}
	case 2:

		sf::Socket::Status status = socket.connect("127.0.0.1", 53000);
		if (status != sf::Socket::Done)
		{
			sf::Socket::Error;
		}

		socket.setBlocking(false);
		selector.add(socket);

		m_gridManager->resetGrid(m_server->playerGrid, m_server->player2Grid, m_gridWidth, m_gridHeight);

		while (GetPlaying())
		{
			playerInput();
		}
	/*	while (GetPlaying())
		{
			char data[2048];
			std::cin.get(data, 2048);
			std::cin.ignore();
			std::cin.clear();

			sf::Packet packet;
			packet << data;
			socket.send(packet);

			playerInput();
		}*/
	}
}

void Client::EnemyAttack(bool _attackAI)
{
	if (_attackAI)
	{
		if (attackCounter == 0)
		{
			m_enemyXPos = m_enemyPrevX + 1;
		}
		else if (attackCounter == 1)
		{
			m_enemyYPos = m_enemyPrevY + 1;
		}
		else if (attackCounter == 2)
		{
			m_enemyXPos = m_enemyPrevX;
			m_enemyYPos = m_enemyPrevY - 1;
		}
		else if (attackCounter == 3)
		{
			m_enemyXPos = m_enemyPrevX - 1;
			m_enemyYPos = m_enemyPrevY;
		}
		attackCounter++;
		checkHit(playerGrid, m_enemyXPos, m_enemyYPos);
	}
	else
	{
		m_enemyXPos = rand() % 10 + 1;
		m_enemyYPos = rand() % 10 + 1;

		m_enemyPrevX = m_enemyXPos;
		m_enemyPrevY = m_enemyYPos;

		checkHit(playerGrid, m_enemyXPos, m_enemyYPos);
	}
}

//void Client::resetGrid()
//{
//	for (int r = 0; r < m_gridWidth; r++)
//	{
//		for (int c = 0; c < m_gridHeight; c++)
//		{
//			if (c != 0 && r == 0)
//			{
//				playerGrid[c][r] = c - 1;
//				player2Grid[c][r] = c - 1;
//			}
//			else if (r != 0 && c == 0)
//			{
//				playerGrid[c][r] = r - 1;
//				player2Grid[c][r] = r - 1;
//			}
//			else if (r == 0 && c == 0)
//			{
//				playerGrid[c][r] = 32;
//				player2Grid[c][r] = 32;
//			}
//			else
//			{
//				playerGrid[c][r] = 45;
//				player2Grid[c][r] = 45;
//			}
//		}
//	}
//
//	m_carrier = new AircraftCarrier();
//	m_battleship = new Battleship();
//	m_submarine = new Submarine();
//	m_destroyer = new Destroyer();
//	m_patrol = new PatrolBoat();
//}

//void Client::printGrid(int _array[11][11])
//{
//	int letterValue = 65;
//	bool hideS = false;
//
//	if (_array == player2Grid)
//	{
//	//	hideS = true;
//		std::cout << "Enemy Grid" << std::endl;
//	}
//	else
//	{
//		std::cout << "Player Grid" << std::endl;
//	}
//
//	for (int r = 0; r < m_gridWidth; r++)
//	{
//		for (int c = 0; c < m_gridHeight; c++)
//		{
//			if (_array[c][r] == 45)
//			{
//				char temp = 45;
//				std::cout << temp;
//			}
//			else if (_array[c][r] == 32)
//			{
//				char temp = 32;
//				std::cout << temp;
//			}
//			else if (_array[c][r] == 83)
//			{
//				if (!hideS)
//				{
//					char temp = 83;
//					std::cout << temp;
//				}
//				else
//				{
//					char temp = 45;
//					std::cout << temp;
//				}
//			}
//			else if (_array[c][r] == 88)
//			{
//				char temp = 88;
//				std::cout << temp;
//			}
//			else if (_array[c][r] == 79)
//			{
//				char temp = 79;
//				std::cout << temp;
//			}
//			else if (c != 0 && r == 0)
//			{
//				char temp = letterValue;
//				letterValue++;
//				std::cout << temp;
//			}
//			else
//			{
//				std::cout << _array[c][r];
//			}
//			std::cout << " ";
//		}
//		std::cout << std::endl;
//	}
//	std::cout << std::endl;
//}

//void Client::placeShip(int _array[11][11], int& _shipSize, int _shipX, int _shipY, char _direction)
//{
//	if (checkGrid(_array, _direction, _shipSize, _shipX, _shipY))
//	{
//		switch (_direction)
//		{
//		case 'N':
//			for (int i = 0; i < _shipSize; i++)
//			{
//				_array[_shipX][_shipY + i] = 83;
//			}
//			break;
//		case 'S':
//			for (int i = 0; i < _shipSize; i++)
//			{
//				_array[_shipX][_shipY - i] = 83;
//			}
//			break;
//		case 'E':
//			for (int i = 0; i < _shipSize; i++)
//			{
//				_array[_shipX - i][_shipY] = 83;
//			}
//			break;
//		case 'W':
//			for (int i = 0; i < _shipSize; i++)
//			{
//				_array[_shipX + i][_shipY] = 83;
//			}
//			break;
//		}
//	}
//	else
//	{
//		if (!GetEnemy())
//		{
//			std::cout << "Error: Ship placement will cause collision." << std::endl
//				<< "Please enter new co-ordinates." << std::endl;
//
//			m_shipPlaced -= 1;
//
//			checkShip(_shipSize, false);
//		}
//	}
//}

int Client::checkLetter(char _letter)
{
	return _letter - 'A' + 1;
}

//bool Client::checkGrid(int _array[11][11], char _direction, int _shipSize, int _shipX, int _shipY)
//{
//	for (int i = 0; i < _shipSize; i++)
//	{
//		switch (_direction)
//		{
//		case 'N':
//			if (_array[_shipX][_shipY + i] == 83)
//			{
//				return false;
//			}
//
//			if (m_AI)
//			{
//				if (_array[_shipX][_shipY - 1] == 83)
//				{
//					return false;
//				}
//				else if (_array[_shipX][_shipY + _shipSize] == 83)
//				{
//					return false;
//				}
//				else if (_array[_shipX - 1][_shipY + i] == 83)
//				{
//					return false;
//				}
//				else if (_array[_shipX + 1][_shipY + i] == 83)
//				{
//					return false;
//				}
//			}
//
//			if (_shipY + i >= m_gridHeight)
//			{
//				return false;
//			}
//			break;
//		case 'S':
//			if (_array[_shipX][_shipY - i] == 83)
//			{
//				return false;
//			}
//
//			if (m_AI)
//			{
//				if (_array[_shipX][_shipY + 1] == 83)
//				{
//					return false;
//				}
//				else if (_array[_shipX][_shipY - _shipSize] == 83)
//				{
//					return false;
//				}
//				else if (_array[_shipX - 1][_shipY - i] == 83)
//				{
//					return false;
//				}
//				else if (_array[_shipX + 1][_shipY - i] == 83)
//				{
//					return false;
//				}
//			}
//
//			if (_shipY - i < 1)
//			{
//				return false;
//			}
//			break;
//		case 'E':
//			if (_array[_shipX - i][_shipY] == 83)
//			{
//				return false;
//			}
//
//			if (m_AI)
//			{
//				if (_array[_shipX + 1][_shipY] == 83)
//				{
//					return false;
//				}
//				else if (_array[_shipX - _shipSize][_shipY] == 83)
//				{
//					return false;
//				}
//				else if (_array[_shipX - i][_shipY - 1] == 83)
//				{
//					return false;
//				}
//				else if (_array[_shipX - i][_shipY + 1] == 83)
//				{
//					return false;
//				}
//			}
//
//			if (_shipX - i < 1)
//			{
//				return false;
//			}
//			break;
//		case 'W':
//			if (_array[_shipX + i][_shipY] == 83)
//			{
//				return false;
//			}
//
//			if (m_AI)
//			{
//				if (_array[_shipX - 1][_shipY] == 83)
//				{
//					return false;
//				}
//				else if (_array[_shipX + _shipSize][_shipY] == 83)
//				{
//					return false;
//				}
//				else if (_array[_shipX + i][_shipY - 1] == 83)
//				{
//					return false;
//				}
//				else if (_array[_shipX + i][_shipY + 1] == 83)
//				{
//					return false;
//				}
//			}
//
//			if (_shipX + i >= m_gridWidth)
//			{
//				return false;
//			}
//			break;
//		}
//	}
//
//	return true;
//}

void Client::playerInput()
{
	if (GetShipPlaced() >= 0)
	{
		SetPlacement(false);
	}

	if (GetPlacement())
	{
		char playerBoat;

		if (!m_singlePlayer)
		{
			m_gridManager->printGrid(m_server->playerGrid, m_gridWidth, m_gridHeight);
		}
		else
		{
			m_gridManager->printGrid(playerGrid, m_gridWidth, m_gridHeight);
		}

		std::cout << "====================" << std::endl << std::endl;
		std::cout << "Which ship would you like to place? " << std::endl
			<< "1. Patrol Boat" << std::endl
			<< "2. Destroyer" << std::endl
			<< "3. Submarine" << std::endl
			<< "4. Battleship" << std::endl
			<< "5. Aircraft Carrier" << std::endl << std::endl;

		std::cin >> m_shipNumber;

		if (!m_singlePlayer)
		{
			if (m_gridManager->checkShip(m_shipNumber, true, m_shipSize, m_server->m_carrier,
				m_server->m_battleship, m_server->m_submarine, m_server->m_destroyer,
				m_server->m_patrol))
			{

				sf::Packet typePacket;
				sf::Packet shipSize;
				sf::Packet directionPacket;
				sf::Packet xPosPacket;
				sf::Packet yPosPacket;
				sf::Packet placedPacket;

				std::string type = "Place";
				typePacket << type;
				std::string size = std::to_string(m_shipNumber);
				shipSize << size;

				std::cout << "Please enter the direction to face. (N, W, E, or S)" << std::endl;
				std::cin >> m_direction;
				std::cout << std::endl;

				m_direction = toupper(m_direction);
				std::string direction = std::to_string(m_direction);
				directionPacket << direction;

				std::cout << "Please enter the X co-ordinate for your ship (letter)." << std::endl;
				std::cin >> playerBoat;
				std::cout << std::endl;

				playerBoat = toupper(playerBoat);
				m_shipX = checkLetter(playerBoat);
				std::string x = std::to_string(m_shipX);
				xPosPacket << x;

				std::cout << "Please enter the Y co-ordinate for your ship (number)." << std::endl;
				std::cin >> m_shipY;
				std::cout << std::endl;

				m_shipY += 1;

				std::string y = std::to_string(m_shipY);
				yPosPacket << y;

				int shipPlaced = GetShipPlaced();
				std::string p = std::to_string(shipPlaced);
				placedPacket << p;

				socket.send(typePacket);
				socket.send(shipSize);
				socket.send(directionPacket);
				socket.send(xPosPacket);
				socket.send(yPosPacket);
				socket.send(placedPacket);

				shipPlaced += 1;
				SetShipPlaced(shipPlaced);
			}
			else
			{
				std::cout << "Error: Please choose a different ship to place."
					<< std::endl << std::endl;
			}
		}
		else
		{
			if (m_gridManager->checkShip(m_shipNumber, true, m_shipSize, m_carrier,
				m_battleship, m_submarine, m_destroyer,
				m_patrol))
			{
				std::cout << "Please enter the direction to face. (N, W, E, or S)" << std::endl;
				std::cin >> m_direction;
				std::cout << std::endl;

				m_direction = toupper(m_direction);

				std::cout << "Please enter the X co-ordinate for your ship (letter)." << std::endl;
				std::cin >> playerBoat;
				std::cout << std::endl;

				playerBoat = toupper(playerBoat);
				m_shipX = checkLetter(playerBoat);

				std::cout << "Please enter the Y co-ordinate for your ship (number)." << std::endl;
				std::cin >> m_shipY;
				std::cout << std::endl;

				m_shipY += 1;

				int shipSize = GetShipSize();
				int shipPlaced = GetShipPlaced();

				m_gridManager->placeShip(playerGrid, shipSize, m_shipX, m_shipY, m_direction, shipPlaced, false,
					m_carrier, m_battleship, m_submarine, m_destroyer, m_patrol);

				shipPlaced += 1;

				SetShipPlaced(shipPlaced);
			}
			else
			{
				std::cout << "Error: Please choose a different ship to place."
					<< std::endl << std::endl;
			}
		}
	}
	else
	{
		std::cout << std::endl << std::endl
			<< "==========ATTACK=========="
			<< std::endl << std::endl;

		if (!m_singlePlayer)
		{
			m_gridManager->printGrid(m_server->player2Grid, m_server->getWidth(), m_server->getHeight());
		}
		else
		{
			m_gridManager->printGrid(player2Grid, m_gridWidth, m_gridHeight);
		}

		std::cout << "Please enter the X co-ordinate for your shot. (letter)" << std::endl;
		std::cin >> m_xShot;

		std::cout << "Please enter the Y co-ordinate for your shot. (number)" << std::endl;
		std::cin >> m_yShot;

		m_xShot = toupper(m_xShot);

		if (!m_singlePlayer)
		{
			hitMiss(m_server->player2Grid, m_xShot, m_yShot);
		}
		else
		{
			hitMiss(player2Grid, m_xShot, m_yShot);
		}
		
	}
}

void Client::hitMiss(int _array[11][11], char _xShot, int _yShot)
{
	char tempX = checkLetter(_xShot);
	int tempY = _yShot + 1;

	if (!m_singlePlayer)
	{
		sf::Packet typePacket;
		sf::Packet xPacket;
		sf::Packet yPacket;

		std::string type = "Shot";
		std::string x = std::to_string(tempX);
		std::string y = std::to_string(tempY);

		typePacket << type;
		xPacket << x;
		yPacket << y;

		socket.send(typePacket);
		socket.send(xPacket);
		socket.send(yPacket);
	}
	else
	{
		checkHit(_array, tempX, tempY);
	}
}

void Client::initialiseEnemy()
{
	while (m_enemy)
	{
		int xPos = rand() % 10 + 1;
		int yPos = rand() % 10 + 1;

		char direction;
		int num = rand() % 4 + 1;

		switch (num)
		{
		case 1:
			direction = 'N';
			break;
		case 2:
			direction = 'E';
			break;
		case 3:
			direction = 'S';
			break;
		case 4:
			direction = 'W';
			break;
		default:
			break;
		}

		if (m_gridManager->checkGrid(player2Grid, direction, m_enemyShipSize, xPos, yPos, true))
		{
			m_gridManager->placeShip(player2Grid, m_enemyShipSize, xPos, yPos, direction, m_enemyPlaced, 
			 true, m_eCarrier, m_eBattleship, m_eSubmarine, m_eDestroyer, m_ePatrol);

			m_enemyShipSize += 1;

			m_enemyPlaced += 1;
		}
		

		if (m_enemyPlaced >= 5)
		{
			m_enemy = false;
		}
	}

	std::cout << "The enemy grid has been setup."
		<< std::endl << std::endl;
}

void Client::checkHit(int _array[11][11], int _xShot, int _yShot)
{
	if (m_singlePlayer)
	{
		if (!GetTurn())
		{
			if (_array[_xShot][_yShot] == 83)
			{
				std::cout << std::endl << "Hit!" << std::endl << std::endl;
				_array[_xShot][_yShot] = 88;
				m_gridManager->printGrid(playerGrid, m_gridWidth, m_gridHeight);
				SetTurn(false);
				EnemyAttack(true);
			}
			else
			{
				std::cout << std::endl << "Miss!" << std::endl << std::endl;
				_array[_xShot][_yShot] = 79;
				m_gridManager->printGrid(playerGrid, m_gridWidth, m_gridHeight);
				SetTurn(true);
			}
		}
		else
		{
			if (_array[_xShot][_yShot] == 83)
			{
				std::cout << std::endl << "Hit!" << std::endl << std::endl;
				_array[_xShot][_yShot] = 88;
				m_gridManager->printGrid(player2Grid, m_gridWidth, m_gridHeight);
				SetTurn(true);
			}
			else
			{
				std::cout << std::endl << "Miss!" << std::endl << std::endl;
				_array[_xShot][_yShot] = 79;
				m_gridManager->printGrid(player2Grid, m_gridWidth, m_gridHeight);
				SetTurn(false);
				EnemyAttack(false);
			}
		}
	}
	else
	{
		auto handle = std::async(std::launch::async, [_array, _xShot, _yShot] {

			sf::Packet packet;
			if (selector.wait() && selector.isReady(socket))
			{
				if (socket.receive(packet) == sf::Socket::Done)
				{
					//convert to string
					std::string s;
					packet >> s;

					if (s == "hit")
					{
						//update hit
						_array[_xShot][_yShot] = 88;
					}
					if (s == "miss")
					{
						//update miss
						_array[_xShot][_yShot] = 79;
					}

					std::cout << "> " << s << std::endl;
				}
			}

		});
	}
}

//void Client::checkShip(int _number, bool _set)
//{
//	switch (_number)
//	{
//	case 1:
//		if (!m_patrol->GetPlaced())
//		{
//			m_shipSize = m_patrol->GetSize();
//			m_patrol->SetPlaced(true);
//			m_placeable = true;
//		}
//		else
//		{
//			if (!_set)
//			{
//				m_patrol->SetPlaced(_set);
//			}
//			else
//			{
//				m_placeable = false;
//			}
//		}
//		break;
//	case 2:
//		if (!m_destroyer->GetPlaced())
//		{
//			m_shipSize = m_destroyer->GetSize();
//			m_destroyer->SetPlaced(true);
//			m_placeable = true;
//		}
//		else
//		{
//			if (!_set)
//			{
//				m_destroyer->SetPlaced(_set);
//			}
//			else
//			{
//				m_placeable = false;
//			}
//		}
//		break;
//	case 3:
//		if (!m_submarine->GetPlaced())
//		{
//			m_shipSize = m_submarine->GetSize();
//			m_submarine->SetPlaced(true);
//			m_placeable = true;
//		}
//		else
//		{
//			if (!_set)
//			{
//				m_submarine->SetPlaced(_set);
//			}
//			else
//			{
//				m_placeable = false;
//			}
//		}
//		break;
//	case 4:
//		if (!m_battleship->GetPlaced())
//		{
//			m_shipSize = m_battleship->GetSize();
//			m_battleship->SetPlaced(true);
//			m_placeable = true;
//		}
//		else
//		{
//			if (!_set)
//			{
//				m_battleship->SetPlaced(_set);
//			}
//			else
//			{
//				m_placeable = false;
//			}
//		}
//		break;
//	case 5:
//		if (!m_carrier->GetPlaced())
//		{
//			m_shipSize = m_carrier->GetSize();
//			m_carrier->SetPlaced(true);
//			m_placeable = true;
//		}
//		else
//		{
//			if (!_set)
//			{
//				m_carrier->SetPlaced(_set);
//			}
//			else
//			{
//				m_placeable = false;
//			}
//		}
//		break;
//	default:
//		m_placeable = false;
//		break;
//	}
//}

int main()
{
	std::unique_ptr<Client> game = std::make_unique<Client>();
	game->startingScreen();

	//std::thread listenThread(Listen);

	//auto handle = std::async(std::launch::async, [] {

	//	while (true)
	//	{
	//		sf::Packet packet;
	//		if (selector.wait() && selector.isReady(socket))
	//		{
	//			if (socket.receive(packet) == sf::Socket::Done)
	//			{
	//				//convert to string
	//				std::string tag;
	//				packet >> tag;

	//				if (tag == "ping")
	//				{
	//					//send relpy
	//					sf::Packet pong;
	//					pong << "pong";
	//					socket.send(pong);
	//				}

	//				if (tag == "hit")
	//				{
	//					//update hit
	//					//_enemyGrid[11][11] = 88;
	//				}
	//				if (tag == "miss")
	//				{
	//					//update miss
	//					//_enemyGrid[11][11] = 79;
	//				}

	//				std::cout << "> " << tag << std::endl;
	//			}
	//		}
	//	}
	//});

	return 0;
}
