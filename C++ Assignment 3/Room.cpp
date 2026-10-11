#include <iostream>

#include "Room.h"

Room::Room(int w, int h, char d, int dX) {
	width = w;
	height = h;
	doorSymbol = d;
	doorX = (dX >= 0 && dX < w) ? dX : w / 2;
}

int Room::GetWidth() const {
	return width;
}

int Room::GetHeight() const {
	return height;
}

bool Room::IsInsideRoom(int x, int y) const {
	return x >= 0 && x < width && y >= 0 && y < height;
}

// Display room in grid
void Room::Display(const std::vector<const Entity*>& entities) const {
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			char tile = '.';
			if (IsDoor(x, y)) tile = doorSymbol;
			for (const Entity* e : entities) {
				if (e->GetX() == x && e->GetY() == y) {
					tile = e->GetSymbol();
				}
			}
			std::cout << tile;
		}
		std::cout << "\n\t\t";
	}
}

int Room::GetDoorX() const{
	return doorX;
}

int Room::GetDoorY() const{
	return 0;
}

bool Room::IsDoor(int x, int y) const {
	return x == GetDoorX() && y == GetDoorY();
}