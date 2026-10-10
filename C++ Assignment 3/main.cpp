#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "Room.h"
#include "Player.h"
#include "Enemy.h"

const int ROOMS_BEFORE_BOSS = 3;

void SetupRoom(int roomNumber, Room& room, Player& player, std::vector<Enemy>& enemies)
{
	enemies.clear();

	if (roomNumber <= ROOMS_BEFORE_BOSS) {
		int widths[] = { 7, 9, 11 };
		int heights[] = { 5, 5, 6 };
		int width = widths[roomNumber - 1];
		int height = heights[roomNumber - 1];
		room = Room(width, height, 'D');

		int count = 1 + std::rand() % 2;    // 1 to 3 enemies
		while ((int)enemies.size() < count) {
			int x = std::rand() % width;    // width, not height: enemies are on the top row
			if (x == room.GetDoorX()) continue;

			bool taken = false;
			for (const Enemy& e : enemies) {
				if (e.GetX() == x) taken = true;
			}
			if (taken) continue;

			enemies.push_back(Enemy(x, 0));
		}
	}
	else {
		room = Room(11, 6, 'X');            // boss room, same size as room 3
		enemies.push_back(Enemy(room.GetWidth() / 2 - 2, 0, 200, 'B'));
	}

	player.SetPosition(room.GetWidth() / 2, room.GetHeight() - 1);
}

int main()
{
	std::srand((unsigned)std::time(nullptr));

	int roomNumber = 1;
	Room room(7, 7);
	Player player(0, 0);
	std::vector<Enemy> enemies;
	SetupRoom(roomNumber, room, player, enemies);

	bool running = true;
	while (running)
	{
		std::vector<const Entity*> entities = { &player };
		for (const Enemy& e : enemies) entities.push_back(&e);

		std::cout << "\nRoom " << roomNumber << "\n";
		room.Display(entities);
		std::cout << "Move (w/a/s/d, q to quit): ";

		char input;
		std::cin >> input;

		int dx = 0, dy = 0;
		if (input == 'w') dy = -1;
		else if (input == 's') dy = 1;
		else if (input == 'a') dx = -1;
		else if (input == 'd') dx = 1;
		else if (input == 'q') running = false;

		if (room.IsInsideRoom(player.GetX() + dx, player.GetY() + dy))
			player.Move(dx, dy);

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
	}
	return 0;
}