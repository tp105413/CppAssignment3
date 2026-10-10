#include <iostream>
#include <cstdlib>

#include "Enemy.h"
#include "Player.h"
#include "Room.h"

Enemy::Enemy(int x, int y, int hp, char symbol) : Entity(x, y, hp, symbol), state(EnemyState::Wander) {}

void Enemy::UpdateState(const Player& player) {
	int distance = std::abs(player.GetX() - x) + std::abs(player.GetY() - y);

	if (distance <= GetAttackRange()) {
		state = EnemyState::Attack;
	}
	else if (distance <= GetDetectRange()) {
		state = EnemyState::Chase;
	}
	else {
		state = EnemyState::Wander;
	}
}

void Enemy::TakeTurn(Player& player, const Room& room, const EnemyList& enemies) {
	UpdateState(player);

	switch (state) {
	case EnemyState::Wander:
		Wander(player, room, enemies);
		break;
	case EnemyState::Chase:
		MoveToward(player, room, enemies);
		break;
	case EnemyState::Attack:
		player.TakeDamage(GetDamage());
		std::cout << symbol << " hits you for " << GetDamage() << " damage!\n";
		break;
	}
}

void Enemy::Wander(const Player& player, const Room& room, const EnemyList& enemies) {
	int stepX = 0, stepY = 0;
	int direction = std::rand() % 4;

	if (direction == 0) stepY = -1;
	else if (direction == 1) stepY = 1;
	else if (direction == 2) stepX = -1;
	else stepX = 1;

	TryStep(stepX, stepY, player, room, enemies);
}

void Enemy::MoveToward(const Player& player, const Room& room, const EnemyList& enemies) {
	int dx = player.GetX() - x;
	int dy = player.GetY() - y;

	int stepX = (dx > 0) - (dx < 0);
	int stepY = (dy > 0) - (dy < 0);

	int firstX = 0, firstY = 0, secondX = 0, secondY = 0;
	if (std::abs(dx) >= std::abs(dy)) {
		firstX = stepX;
		secondY = stepY;
	}
	else {
		firstY = stepY;
		secondX = stepX;
	}

	if (TryStep(firstX, firstY, player, room, enemies)) return;
	TryStep(secondX, secondY, player, room, enemies);
}

bool Enemy::TryStep(int stepX, int stepY, const Player& player, const Room& room, const EnemyList& enemies) {
	if (stepX == 0 && stepY == 0) return false;

	int newX = x + stepX;
	int newY = y + stepY;

	if (!room.IsInsideRoom(newX, newY)) return false;

	// Enemy will not block the door
	if (room.IsDoor(newX, newY)) return false;
	if (newX == player.GetX() && newY == player.GetY()) return false;

	for (const auto& other : enemies) {
		if (other.get() != this && other->IsAlive()
			&& other->GetX() == newX && other->GetY() == newY) {
			return false;
		}
	}

	Move(stepX, stepY);
	return true;
}
