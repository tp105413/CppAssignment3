#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

#include "GameManager.h"

using namespace std;

void ShowStatus(Player& player, const EnemyList& enemies)
{
	cout << "HP: " << player.GetHp() << "\n";
	for (int i = 0; i < player.GetSkillCount(); i++) {
		Skill& s = player.GetSkill(i);
		cout << "[" << i + 1 << "] " << s.GetName() << ": ";
		if (s.IsReady()) cout << "READY";
		else cout << "cooldown " << s.GetCurrentCooldown();
		cout << "   ";
	}
	cout << "\n";
	for (const auto& e : enemies) {
		cout << e->GetSymbol() << " (" << e->GetX() << "," << e->GetY()
			<< ") HP " << e->GetHp() << "\n";
	}
}

void StartGame()
{
	GameManager& gm = GameManager::GetInstance();
	gm.StartNewGame();

	bool running = true;
	while (running)
	{
		Player& player = gm.GetPlayer();
		Room& room = gm.GetRoom();
		EnemyList& enemies = gm.GetEnemies();

		vector<const Entity*> entities = { &player };
		for (const auto& e : enemies) entities.push_back(e.get());

		cout << "\nLevel " << gm.GetLevel() << "/" << gm.GetLevelCount()
			<< "   Turns left: " << gm.GetTurnsLeft() << "\n";
		room.Display(entities);
		ShowStatus(player, enemies);
		cout << "w/a/s/d = move or attack, 1/2 = skills, q = quit: ";

		char input;
		cin >> input;

		bool turnUsed = false;

		if (input == 'q') {
			running = false;
		}
		else if (input == '1' || input == '2') {
			Skill& skill = player.GetSkill(input - '1');
			if (!skill.IsReady()) {
				cout << "Skill is on cooldown!\n";
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

				Enemy* target = gm.FindEnemyAt(targetX, targetY);
				if (target) {
					target->TakeDamage(10);
					cout << "Basic attack! 10 damage.\n";
					turnUsed = true;
				}
				else if (room.IsInsideRoom(targetX, targetY)) {
					player.Move(dx, dy);
					turnUsed = true;
				}
			}
		}

		if (turnUsed) {
			gm.RemoveDeadEnemies();
			player.TickSkills();
			gm.UseTurn();

			if (room.IsDoor(player.GetX(), player.GetY())) {
				if (gm.IsLastLevel()) {
					cout << "You escaped the dungeon! Congratulations!\n";
					running = false;
				}
				else {
					gm.NextLevel();
				}
			}
			else if (gm.GetTurnsLeft() <= 0) {
				cout << "You ran out of turns and were buried beneath the dungeon, lost to darkness forever...\n";
				running = false;
			}
			else {
				gm.EnemiesTakeTurn();
				if (!player.IsAlive()) {
					cout << "You died in the dungeon, no longer to see the light of day again...\n";
					running = false;
				}
			}
		}
	}

	cout << "\nPress Enter to return to the menu...";
	cin.ignore(1000, '\n');
	cin.get();
}

int main() {
	srand((unsigned)time(nullptr));

	int choice;

	do {
		system("cls");
		cout << "1. Start Game\n";
		cout << "2. Quit\n\n";
		cout << "Choose: ";
		cin >> choice;
		cin.ignore(1000, '\n');

		if (choice == 1) {
			system("cls");
			StartGame();
		}
	} while (choice != 2);

	exit(0);

	return 0;
}