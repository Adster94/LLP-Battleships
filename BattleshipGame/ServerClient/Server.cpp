// ServerClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
//#include <SFML/Network.hpp>
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <ctime>
#include "Server.h"
#include "Client.h"

const int SERVER_PORT(53000);
//std::vector<sf::TcpSocket*> sockets;

//defines our clock
typedef std::chrono::high_resolution_clock Clock;
Server* server = new Server();
//auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

void checkHit(int _array[11][11], int _xShot, int _yShot);

// Prototypes
int connectedClients = 0;
int sendWelcomeMessage(sf::TcpSocket* client);

int main()
{
	//std::unique_ptr<Server> server = std::make_unique<Server>();

	server->start();

    return 0;
}

void Server::start()
{
	// create a thread
	std::thread thread(&Server::launchServer, this);

	// then loop 
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}

int Server::launchServer()
{
	sf::SocketSelector selector;
	GridManager* m_gridManager = new GridManager();
	InputManager* m_inputManager = new InputManager(m_gridManager);

	m_gridManager->resetGrid(playerGrid, player2Grid, getWidth(), getHeight());
	
	sf::TcpListener listener;
	if (listener.listen(SERVER_PORT) != sf::Socket::Done)
	{
		// throw error
		return -1;
	}

	selector.add(listener);

	// accept connections
	while (true)
	{
		if (selector.wait(sf::milliseconds(500)))
		{
			if (selector.isReady(listener))
			{
				sf::TcpSocket* client = new sf::TcpSocket;
				if (listener.accept(*client) != sf::Socket::Done)
				{
					// throw error
					sf::Socket::Error;
					delete client;
					return -2;
				}
				else
				{
					connectedClients++;
					std::cout << connectedClients;
					sendWelcomeMessage(client);
					sockets.push_back(client);
					selector.add(*client);
				}
			}
			else
			{
				std::vector<sf::TcpSocket*> dead;
				for (auto* client : sockets)
				{
					if (selector.isReady(*client))
					{
						sf::Packet packet;
						if (client->receive(packet) == sf::Socket::Disconnected)
						{
							// stop monitoring
							connectedClients--;
							selector.remove(*client);
							
							// disconnect and record dead client
							client->disconnect();
							dead.push_back(client);

							std::cout << "Client disconnected." << std::endl;
						}
						else
						{
							std::string s;
							packet >> s;
							std::cout << s << std::endl;	
						}

						for (auto* rec : sockets)
						{
							if (rec == client) continue;
							rec->send(packet);
						}

						break;
					}
				}

				for (auto it = dead.begin(); it != dead.end(); ++it)
				{
					sockets.erase(std::find(sockets.begin(), sockets.end(), *it));
					delete *it;
					*it = nullptr;
				}
			}
		}
		else
		{
			printConnectedClients();
			handleInput(sockets);
		}
	}

	return 0;
}

int Server::sendWelcomeMessage(sf::TcpSocket* client)
{
	// on sending side
	sf::Uint16 x = connectedClients;
	std::string s = "Welcome to the Battleships chat room\n> There are " + std::to_string(connectedClients) + " connected clients";
	
	sf::Packet packet;
	packet << s;

	if (client->send(packet) == sf::Socket::Done)
	{
		return 0;
	}

	return 1;
}

void Server::printConnectedClients()
{
	system("cls");
	std::string s = "There are " + std::to_string(connectedClients) + " connected clients";
	std::cout << s << std::endl;

	std::cout << std::endl << std::endl
		<< "Player 1 Grid:" << std::endl;
	m_gridManager->printGrid(playerGrid, getWidth(), getHeight());

	std::cout << std::endl << std::endl
		<< "Player 2 Grid:" << std::endl;
	m_gridManager->printGrid(player2Grid, getWidth(), getHeight());
}

void Server::handleInput(std::vector<sf::TcpSocket*>& _sockets)
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
				checkHit(player2Grid, xShot, yShot, _sockets);
				break;
			case 2:
				checkHit(playerGrid, xShot, yShot, _sockets);
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
			int temp = std::stoi(direction);
			char dir = temp;

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
				m_gridManager->placeShip(playerGrid, shipSize, x, y, dir, p,
					false, m_carrier, m_battleship, m_submarine,
					m_destroyer, m_patrol);
				break;
			case 2:
				m_gridManager->placeShip(player2Grid, shipSize, x, y, dir, p,
					false, m_2Carrier, m_2Battleship, m_2Submarine,
					m_2Destroyer, m_2Patrol);
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

void Server::checkHit(int _array[11][11], int _xShot, int _yShot,
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
					m_gridManager->printGrid(playerGrid, getWidth(), getHeight());
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
					m_gridManager->printGrid(playerGrid, getWidth(), getHeight());
					setTurn(true);
				}
			}
			else
			{
				if (_array[_xShot][_yShot] == 83)
				{
					std::cout << std::endl << "AI Hit!" << std::endl << std::endl;
					_array[_xShot][_yShot] = 88;
					m_gridManager->printGrid(player2Grid, getWidth(), getHeight());
					setTurn(true);
				}
				else
				{
					std::cout << std::endl << "AI Miss!" << std::endl << std::endl;
					_array[_xShot][_yShot] = 79;
					m_gridManager->printGrid(player2Grid, getWidth(), getHeight());
					setTurn(false);
					m_aiManager->enemyAttack(false);
				}
			}
		}
	}
}


