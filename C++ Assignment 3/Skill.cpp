#include <iostream>

#include "Skill.h"
#include "Enemy.h"
#include "Player.h"
#include "Room.h"

Skill::Skill(const std::string& n, int cd) {
	name = n;
	cooldown = cd;
	currentCooldown = 0;
}

const std::string& Skill::GetName() const {
	return name;
}

int Skill::GetCurrentCooldown() const {
	return currentCooldown;
}

bool Skill::IsReady() const {
	return currentCooldown == 0;
}

void Skill::StartCooldown() {
	currentCooldown = cooldown + 1;
}

void Skill::Tick() {
	if (currentCooldown > 0) {
		currentCooldown--;
	}
}

// Spin Slash Skill
SpinSkill::SpinSkill():Skill("Spin Slash", 2){}

bool SpinSkill::Use(Player& player, EnemyList& enemies, const Room&) {
	for (auto& e : enemies) {
		int dx = e->GetX() - player.GetX();
		int dy = e->GetY() - player.GetY();
		if (dx >= -1 && dx <= 1 && dy >= -1 && dy <= 1) {
			e->TakeDamage(20);
		}
	}
	return true;
}

// Dash Skill
DashSkill::DashSkill():Skill("Dash", 3){}

bool DashSkill::Use(Player& player, EnemyList& enemies, const Room& room) {
	std::cout << "Dash direction (w/a/s/d): ";
	char input;
	std::cin >> input;

	int dx = 0, dy = 0;
	if (input == 'w') dy = -1;
	else if (input == 's') dy = 1;
	else if (input == 'a') dx = -1;
	else if (input == 'd') dx = 1;
	else {
		std::cout << "Invalid direction.\n";
		return false;
	}

	for (int step = 0; step < 3; step++) {
		int nextX = player.GetX() + dx;
		int nextY = player.GetY() + dy;
		if (!room.IsInsideRoom(nextX, nextY)) break;

		// Cant dash through enemy if still alive
		bool blocked = false;
		for (auto& e : enemies) {
			if (e->IsAlive() && e->GetX() == nextX && e->GetY() == nextY) {
				e->TakeDamage(30);
				if (e->IsAlive()) blocked = true;
			}
		}
		if (blocked) break;

		player.Move(dx, dy);
	}
	return true;
}