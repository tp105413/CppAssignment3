#pragma once

#include "Enemy.h"

class Undead : public Enemy {
public:
	Undead(int x, int y) : Enemy(x, y, 30, 'U') {}

	std::string GetName() const override {
		return "Undead";
	}

	int GetDamage() const override {
		return 10;
	}

	int GetAttackRange() const override {
		return 1;
	}

	int GetDetectRange() const override {
		return 4;
	}
};

class UndeadBoss : public Enemy {
public:
	UndeadBoss(int x, int y) : Enemy(x,y, 100, 'B'){}
	
	std::string GetName() const override {
		return "Undead Boss";
	}

	int GetDamage() const override {
		return 20;
	}

	int GetAttackRange() const override {
		return 3;
	}

	int GetDetectRange() const override {
		return 20;
	}
};