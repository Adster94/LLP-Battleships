#pragma once

#include "GridManager.h"
#include "InputManager.h"
#include "Server.h"

class Server;
class InputManager;

class AIManager
{
public:
	AIManager(Server* _server)
	{
		m_server = _server;
	}

	void enemyAttack(bool _attackAI);
	void initialiseEnemy();

	bool GetEnemy() { return m_enemy; }
	void SetEnemy(bool _enemy) { m_enemy = _enemy; }

	bool GetEnemyPlaceable() { return m_enemyPlaceable; }
	void SetEnemyPlaceable(bool _placeable) { m_enemyPlaceable = _placeable; }

	bool GetAI() { return m_AI; }
	void SetAI(bool _ai) { m_AI = _ai; }

private:
	bool m_enemy = true;
	bool m_enemyPlaceable = false;
	bool m_AI = false;

	int m_enemyShipSize = 1;
	int m_enemyPlaced = 0;
	int m_enemyPrevX = 0;
	int m_enemyPrevY = 0;
	int m_enemyXPos = 0;
	int m_enemyYPos = 0;

	int attackCounter = 0;

	GridManager* m_gridManager;
	Server* m_server;
	InputManager* m_inputManager;
};