#pragma once

#include <string>
#include <vector>

#include "Enemy.h"

class Player;
class Room;

class Skill {
protected:
	std::string name;
	int cooldown;
	int currentCooldown;

public:
	Skill(const std::string& n, int cd);
	virtual ~Skill() = default;

	const std::string& GetName() const;
	int GetCurrentCooldown() const;
	bool IsReady() const;
	void StartCooldown();
	void Tick();

	virtual bool Use(Player& player, EnemyList& enemies, const Room& room) = 0;
};

class SpinSkill :public Skill {
public:
	SpinSkill();
	bool Use(Player& player, EnemyList& enemies, const Room& room) override;
};

class DashSkill :public Skill {
public:
	DashSkill();
	bool Use(Player& player, EnemyList& enemies, const Room& room) override;
};