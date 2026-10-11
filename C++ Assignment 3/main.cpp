#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

#include "GameManager.h"

using namespace std;

void DisplayScreen(GameManager& gm, bool showControls)
{
	system("cls");

	Player& player = gm.GetPlayer();
	Room& room = gm.GetRoom();
	EnemyList& enemies = gm.GetEnemies();

	// Display combat status message
	for (const string& message : gm.GetMessages()) {
		cout << message << "\n";
	}

	cout << "\n";
	cout << string(50, '=');
	cout << "\nP-Player  U-Undead  B-Undead Boss  D-Door";
	cout << "\n\n\t\t";

	// Display room grid
	vector<const Entity*> entities = { &player };
	for (const auto& e : enemies) entities.push_back(e.get());
	room.Display(entities);

	cout << "\n";
	cout << string(50, '-');
	cout << "\n";

	// Display room and player status
	cout << "Room " << gm.GetLevel() << "/" << gm.GetLevelCount() << "  Turns left: " << gm.GetTurnsLeft() << "\n";
	cout << "Enemies: ";
	if (enemies.empty()) {
		cout << "none";
	}
	for (const auto& e : enemies) {
		cout << e->GetName() << "(" << e->GetHp() << "HP) ";
	}


	cout << "\n\nHP: " << player.GetHp() << "\n";

	for (int i = 0; i < player.GetSkillCount(); i++) {
		Skill& s = player.GetSkill(i);
		cout << "[" << i + 1 << "] " << s.GetName();
		if (s.IsReady()) cout << "(READY)";
		else cout << "(cd:" << s.GetCurrentCooldown()<<")";
		cout << " - " << s.GetDescription();
		cout << "\n";
	}

	cout << "\n";

	// Display player input
	if (showControls) {
		cout << "(w/a/s/d) = move or attack\n(1/2) = skills";
		cout << "\nAction: ";
	}
}

void RunGame()
{
	GameManager& gm = GameManager::GetInstance();
	gm.StartNewGame();

	string endMessage;
	bool running = true;

	while (running)
	{
		Player& player = gm.GetPlayer();
		Room& room = gm.GetRoom();
		EnemyList& enemies = gm.GetEnemies();

		DisplayScreen(gm, true);

		char input;
		cin >> input;
		gm.ClearMessages();

		bool turnUsed = false;

		if (input == '1' || input == '2') {
			Skill& skill = player.GetSkill(input - '1');
			if (!skill.IsReady()) {
				gm.AddMessage("Skill is on cooldown!");
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
					gm.AddMessage("You hit " + target->GetName() + " for 10 damage!");
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

			// Win condition: Player reaches last level door
			if (room.IsDoor(player.GetX(), player.GetY())) {
				if (gm.IsLastLevel()) {
					endMessage = "You escaped the dungeon! Congratulations!\n";
					running = false;
				}
				else {
					gm.NextLevel();
				}
			}
			// Lose condition: Turns run out
			else if (gm.GetTurnsLeft() <= 0) {
				endMessage = "You ran out of turns and were buried beneath the dungeon, lost to darkness forever...\n";
				running = false;
			}
			// Lose condition: Player health reaches 0
			else {
				gm.EnemiesTakeTurn();
				if (!player.IsAlive()) {
					endMessage = "You died in the dungeon, no longer to see the light of day again...\n";
					running = false;
				}
			}
		}
	}

	DisplayScreen(gm, false);
	cout << "\n" << endMessage << "\n";


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
			RunGame();
		}
	} while (choice != 2);

	exit(0);

	return 0;
}