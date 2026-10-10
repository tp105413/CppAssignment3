#include "Player.h"

Player::Player(int x, int y) : Entity(x, y, 100, 'P') {
	skills.push_back(std::make_unique<SpinSkill>());
	skills.push_back(std::make_unique<DashSkill>());
}

int Player::GetSkillCount() const {
	return (int)skills.size();
}

Skill& Player::GetSkill(int index) {
	return *skills[index];
}

void Player::TickSkills() {
	for (auto& skill : skills) {
		skill->Tick();
	}
}