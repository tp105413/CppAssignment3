#include <algorithm>
#include <cstdlib>

#include "GameManager.h"
#include "EnemyFactory.h"

namespace {
	struct LevelData {
		int width;
		int height;
		int turnLimit;
		bool isBoss;
	};

	const LevelData levels[] = {
		{  7, 5, 10, false },
		{  8, 5, 10, false },
		{ 9, 6, 10, false },
		{ 10, 6, 9, false },
		{ 11, 8, 8, true  },
	};

	const int levelCount = sizeof(levels) / sizeof(levels[0]);

	const int minEnemies = 1;
	const int maxEnemies = 3;
	const int healAmount = 10;
}

GameManager::GameManager() : level(1), turnsLeft(0), room(7, 5) {}

// Create GameManager once for the first time
GameManager& GameManager::GetInstance() {
	static GameManager instance;
	return instance;
}

void GameManager::StartNewGame() {
	player = std::make_unique<Player>(0, 0);
	SetupLevel(1);
}

void GameManager::NextLevel() {
	SetupLevel(level + 1);
}

void GameManager::SetupLevel(int newLevel) {
	level = newLevel;
	const LevelData& data = levels[level - 1];

	enemies.clear();

	// Randomise the door position
	int doorX = std::rand() % data.width;
	room = Room(data.width, data.height, 'D', doorX);
	turnsLeft = data.turnLimit;

	if (data.isBoss) {
		int bossX = room.GetWidth() / 2 - 2;

		if (bossX == room.GetDoorX()) {
			bossX = room.GetWidth() / 2 + 2;
		}
		enemies.push_back(EnemyFactory::Create("boss", bossX, 0));
	}
	else {
		int count = minEnemies + std::rand() % (maxEnemies - minEnemies + 1);
		while ((int)enemies.size() < count) {
			int x = std::rand() % data.width;
			if (x == room.GetDoorX()) continue;

			bool taken = false;
			for (const auto& e : enemies) {
				if (e->GetX() == x) taken = true;
			}
			if (taken) continue;

			enemies.push_back(EnemyFactory::Create("undead", x, 0));
		}
	}

	player->SetPosition(room.GetWidth() / 2, room.GetHeight() - 1);
}

int GameManager::GetLevel() const {
	return level;
}

int GameManager::GetLevelCount() const {
	return levelCount;
}

bool GameManager::IsLastLevel() const {
	return level == levelCount;
}

int GameManager::GetTurnsLeft() const {
	return turnsLeft;
}

void GameManager::UseTurn() {
	turnsLeft--;
}

Player& GameManager::GetPlayer() {
	return *player;
}

Room& GameManager::GetRoom() {
	return room;
}

EnemyList& GameManager::GetEnemies() {
	return enemies;
}

Enemy* GameManager::FindEnemyAt(int x, int y) {
	for (auto& e : enemies) {
		if (e->IsAlive() && e->GetX() == x && e->GetY() == y) return e.get();
	}
	return nullptr;
}

void GameManager::RemoveDeadEnemies() {

	for (const auto& e : enemies) {
		if (!e->IsAlive()) {
			player->Heal(healAmount);
			AddMessage(e->GetName() + " defeated! +" + std::to_string(healAmount) + " HP");
		}
	}

	enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
		[](const std::unique_ptr<Enemy>& e) { return !e->IsAlive(); }), enemies.end());
}

void GameManager::EnemiesTakeTurn() {
	for (auto& e : enemies) {
		e->TakeTurn(*player, room, enemies);
	}
}

void GameManager::AddMessage(const std::string& message) {
	messages.push_back(message);
}

void GameManager::ClearMessages() {
	messages.clear();
}

const std::vector<std::string>& GameManager::GetMessages() const {
	return messages;
}