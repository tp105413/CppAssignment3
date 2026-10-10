#include "EnemyFactory.h"
#include "EnemyTypes.h"

std::unique_ptr<Enemy> EnemyFactory::Create(const std::string& type, int x, int y) {
	if (type == "undead") return std::make_unique<Undead>(x, y);
	if (type == "boss")   return std::make_unique<UndeadBoss>(x, y);
	return nullptr;
}