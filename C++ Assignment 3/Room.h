#pragma once

#include <vector>

#include "Entity.h"

class Room {
private:
	int width;
	int height;
	char doorSymbol;

public:
	Room(int w, int h, char d = 'D');

	int GetWidth() const;
	int GetHeight() const;
	bool IsInsideRoom(int x, int y) const;
	void Display(const std::vector<const Entity*>& entities) const;

	int GetDoorX() const;
	int GetDoorY() const;
	bool IsDoor(int x, int y) const;
};