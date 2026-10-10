#pragma once

#include <vector>
#include <memory>

#include "Entity.h"
#include "Skill.h"

class Player : public Entity {
private:
	std::vector<std::unique_ptr<Skill>> skills;

public:
	Player(int x, int y);

	int GetSkillCount() const;
	Skill& GetSkill(int index);
	void TickSkills();
};