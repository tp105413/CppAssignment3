#pragma once

#include <string>
#include <vector>

#include "Room.h"
#include "Player.h"
#include "Enemy.h"

class GameManager {
private:
	int level;
	int turnsLeft;
	Room room;
	std::unique_ptr<Player> player;
	EnemyList enemies;

	GameManager();
	void SetupLevel(int newLevel);

	std::vector<std::string> messages;

public:
	static GameManager& GetInstance();

	GameManager(const GameManager&) = delete;
	GameManager& operator =(const GameManager&) = delete;

	void StartNewGame();
	void NextLevel();
	int GetLevel() const;
	int GetLevelCount() const;
	bool IsLastLevel() const;
	int GetTurnsLeft() const;
	void UseTurn();

	Player& GetPlayer();
	Room& GetRoom();
	EnemyList& GetEnemies();
	Enemy* FindEnemyAt(int x, int y);
	void RemoveDeadEnemies();
	void EnemiesTakeTurn();

	void AddMessage(const std::string& message);
	void ClearMessages();
	const std::vector<std::string>& GetMessages() const;
};