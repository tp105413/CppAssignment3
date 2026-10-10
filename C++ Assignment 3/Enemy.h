#pragma once

#include "Entity.h"

class Enemy : public Entity {
public:
	Enemy(int x, int y, int hp = 30, char symbol = 'E');
};