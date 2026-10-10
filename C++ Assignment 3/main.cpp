#include <iostream>
#include <vector>
#include <memory>
#include <cstdlib>
#include <ctime>
#include <algorithm>

#include "Room.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyFactory.h"

const int ROOMS_BEFORE_BOSS = 3;

void SetupRoom(int roomNumber, Room& room, Player& player, EnemyList& enemies)
{
	enemies.clear();

	if (roomNumber <= ROOMS_BEFORE_BOSS) {
		int widths[] = { 7, 9, 11 };
		int heights[] = { 5, 5, 6 };
		int width = widths[roomNumber - 1];
		int height = heights[roomNumber - 1];
		room = Room(width, height, 'D');

		int count = 1 + std::rand() % 2;    // 1 to 2 enemies
		while ((int)enemies.size() < count) {
			int x = std::rand() % width;
			if (x == room.GetDoorX()) continue;

			bool taken = false;
			for (const auto& e : enemies) {
				if (e->GetX() == x) taken = true;
			}
			if (taken) continue;

			enemies.push_back(EnemyFactory::Create("undead", x, 0));
		}
	}
	else {
		room = Room(11, 6, 'X');            // boss room
		enemies.push_back(EnemyFactory::Create("boss", room.GetWidth() / 2 - 2, 0));
	}

	player.SetPosition(room.GetWidth() / 2, room.GetHeight() - 1);
}

Enemy* FindEnemyAt(EnemyList& enemies, int x, int y)
{
	for (auto& e : enemies) {
		if (e->IsAlive() && e->GetX() == x && e->GetY() == y) return e.get();
	}
	return nullptr;
}

void RemoveDeadEnemies(EnemyList& enemies)
{
	enemies.erase(
		std::remove_if(enemies.begin(), enemies.end(),
			[](const std::unique_ptr<Enemy>& e) { return !e->IsAlive(); }),
		enemies.end());
}

void ShowStatus(Player& player, const EnemyList& enemies)
{
	std::cout << "HP: " << player.GetHp() << "\n";
	for (int i = 0; i < player.GetSkillCount(); i++) {
		Skill& s = player.GetSkill(i);
		std::cout << "[" << i + 1 << "] " << s.GetName() << ": ";
		if (s.IsReady()) std::cout << "READY";
		else std::cout << "cooldown " << s.GetCurrentCooldown();
		std::cout << "   ";
	}
	std::cout << "\n";
	for (const auto& e : enemies) {
		std::cout << e->GetSymbol() << " (" << e->GetX() << "," << e->GetY()
			<< ") HP " << e->GetHp() << "\n";
	}
}

int main()
{
	std::srand((unsigned)std::time(nullptr));

	int roomNumber = 1;
	Room room(7, 5);
	Player player(0, 0);
	EnemyList enemies;
	SetupRoom(roomNumber, room, player, enemies);

	bool running = true;
	while (running)
	{
		std::vector<const Entity*> entities = { &player };
		for (const auto& e : enemies) entities.push_back(e.get());

		std::cout << "\nRoom " << roomNumber << "\n";
		room.Display(entities);
		ShowStatus(player, enemies);
		std::cout << "w/a/s/d = move or attack, 1/2 = skills, q = quit: ";

		char input;
		std::cin >> input;

		bool turnUsed = false;   // did the player really do something?

		if (input == 'q') {
			running = false;
		}
		else if (input == '1' || input == '2') {
			Skill& skill = player.GetSkill(input - '1');
			if (!skill.IsReady()) {
				std::cout << "Skill is on cooldown!\n";
			}
			else if (skill.Use(player, enemies, room)) {
				skill.StartCooldown();
				turnUsed = true;
			}
		}
		else {
			int dx = 0, dy = 0;
			if (input == 'w') dy = -1;
			else if (input == 's') dy = 1;
			else if (input == 'a') dx = -1;
			else if (input == 'd') dx = 1;

			if (dx != 0 || dy != 0) {
				int targetX = player.GetX() + dx;
				int targetY = player.GetY() + dy;

				Enemy* target = FindEnemyAt(enemies, targetX, targetY);
				if (target) {
					target->TakeDamage(10);               // basic attack
					std::cout << "Basic attack! 10 damage.\n";
					turnUsed = true;
				}
				else if (room.IsInsideRoom(targetX, targetY)) {
					player.Move(dx, dy);
					turnUsed = true;
				}
			}
		}

		if (turnUsed) {
			RemoveDeadEnemies(enemies);
			player.TickSkills();

			if (room.IsDoor(player.GetX(), player.GetY())) {
				if (roomNumber > ROOMS_BEFORE_BOSS) {
					std::cout << "You cleared the dungeon!\n";
					running = false;
				}
				else {
					roomNumber++;
					SetupRoom(roomNumber, room, player, enemies);
				}
			}
			else {
				// enemies act after the player
				for (auto& e : enemies) {
					e->TakeTurn(player, room, enemies);
				}
				if (!player.IsAlive()) {
					std::cout << "You died...\n";
					running = false;
				}
			}
		}
	}

	return 0;
}