#include "InputManager.h"
#include <iostream>
#include <chrono>
#include <ctime>
#include <SFML/Network.hpp>

typedef std::chrono::high_resolution_clock Clock;

void InputManager::handleInput(std::vector<sf::TcpSocket*>& _sockets)
{
	int clientNum = 0;

	for (auto client : _sockets)
	{
		//start clock before we send the packet
		auto start = Clock::now();
		clientNum++;

		//create packet and send to client
		sf::Packet packet;

		packet << "ping";
		client->send(packet);

		//wait for response
		client->receive(packet);
		packet >> type;

		if (type == "Shot")
		{
			client->receive(packet);
			packet >> xShotString;

			client->receive(packet);
			packet >> yShotString;

			int xShot = std::stoi(xShotString);
			int yShot = std::stoi(yShotString);

			switch (clientNum)
			{
			case 1:
				checkHit(m_server->player2Grid, xShot, yShot, _sockets);
				break;
			case 2:
				checkHit(m_server->playerGrid, xShot, yShot, _sockets);
				break;
			default:
				break;
			}
		}
		else if (type == "Place")
		{
			client->receive(packet);
			packet >> ship;
			int shipSize = std::stoi(ship);

			client->receive(packet);
			packet >> direction;
			const char *conversion = direction.c_str();
			char dir = *conversion;

			client->receive(packet);
			packet >> xLocation;
			int x = std::stoi(xLocation);

			client->receive(packet);
			packet >> yLocation;
			int y = std::stoi(yLocation);

			client->receive(packet);
			packet >> placed;
			int p = std::stoi(placed);

			std::cout << type << ship << direction << xLocation << yLocation << placed << std::endl;

			switch (clientNum)
			{
			case 1:
				m_gridManager->placeShip(m_server->playerGrid, shipSize, x, y, dir, p,
					false, m_server->m_carrier, m_server->m_battleship, m_server->m_submarine,
					m_server->m_destroyer, m_server->m_patrol);
				break;
			case 2:
				m_gridManager->placeShip(m_server->player2Grid, shipSize, x, y, dir, p,
					false, m_server->m_2Carrier, m_server->m_2Battleship, m_server->m_2Submarine,
					m_server->m_2Destroyer, m_server->m_2Patrol);
				break;
			}
		}

		auto end = Clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

		//print time taken
		std::cout << "Client" << clientNum << " : " << std::endl
			<< type << std::endl
			<< xShotString << yShotString
			<< std::endl;
	}
}

void InputManager::checkHit(int _array[11][11], int _xShot, int _yShot, 
	std::vector<sf::TcpSocket*>& _sockets)
{
	sf::Packet packet;

	for (auto client : _sockets)
	{
		if (m_aiManager->GetAI())
		{
			if (!getTurn())
			{
				if (_array[_xShot][_yShot] == 83)
				{
					std::cout << std::endl << "Hit!" << std::endl << std::endl;
					packet << "Hit";
					client->send(packet);

					_array[_xShot][_yShot] = 88;
					m_gridManager->printGrid(m_server->playerGrid, m_server->getWidth(), m_server->getHeight());
					//m_gridManager->printGrid(playerGrid, 11, 11);
					setTurn(false);
					m_aiManager->enemyAttack(true);
				}
				else
				{
					std::cout << std::endl << "Miss!" << std::endl << std::endl;
					packet << "Miss";
					client->send(packet);

					_array[_xShot][_yShot] = 79;
					m_gridManager->printGrid(m_server->playerGrid, m_server->getWidth(), m_server->getHeight());
					setTurn(true);
				}
			}
			else
			{
				if (_array[_xShot][_yShot] == 83)
				{
					std::cout << std::endl << "AI Hit!" << std::endl << std::endl;
					_array[_xShot][_yShot] = 88;
					m_gridManager->printGrid(m_server->player2Grid, m_server->getWidth(), m_server->getHeight());
					setTurn(true);
				}
				else
				{
					std::cout << std::endl << "AI Miss!" << std::endl << std::endl;
					_array[_xShot][_yShot] = 79;
					m_gridManager->printGrid(m_server->player2Grid, m_server->getWidth(), m_server->getHeight());
					setTurn(false);
					m_aiManager->enemyAttack(false);
				}
			}
		}
	}
}