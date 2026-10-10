#pragma once

#include <memory>
#include <vector>

#include "Entity.h"

class Player;
class Room;
class Enemy;

using EnemyList = std::vector<std::unique_ptr<Enemy>>;

enum class EnemyState {
	Wander, Chase, Attack
};

class Enemy : public Entity {
protected:
	EnemyState state;

private:
	void UpdateState(const Player& player);
	void Wander(const Player& player, const Room& room, const EnemyList& enemies);
	void MoveToward(const Player& player, const Room& room, const EnemyList& enemies);
	bool TryStep(int stepX, int stepY, const Player& player, const Room& room, const EnemyList& enemies);

public:
	Enemy(int x, int y, int hp, char symbol);

	void TakeTurn(Player& player, const Room& room, const EnemyList& enemies);

	virtual int GetDamage() const = 0;
	virtual int GetAttackRange() const = 0;
	virtual int GetDetectRange() const = 0;
};