#include "AIManager.h"
#include <iostream>

void AIManager::initialiseEnemy()
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

		if (m_gridManager->checkGrid(m_server->player2Grid, direction, m_enemyShipSize, xPos, yPos, true))
		{
			m_gridManager->placeShip(m_server->player2Grid, m_enemyShipSize, xPos, yPos, direction, m_enemyPlaced,
				true, m_server->m_2Carrier, m_server->m_2Battleship, m_server->m_2Submarine, 
				m_server->m_2Destroyer, m_server->m_2Patrol);

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

void AIManager::enemyAttack(bool _attackAI)
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
		m_inputManager->checkHit(m_server->playerGrid, m_enemyXPos, m_enemyYPos, 
			m_server->sockets);
	}
	else
	{
		m_enemyXPos = rand() % 10 + 1;
		m_enemyYPos = rand() % 10 + 1;

		m_enemyPrevX = m_enemyXPos;
		m_enemyPrevY = m_enemyYPos;

		m_inputManager->checkHit(m_server->playerGrid, m_enemyXPos, m_enemyYPos, 
			m_server->sockets);
	}
}