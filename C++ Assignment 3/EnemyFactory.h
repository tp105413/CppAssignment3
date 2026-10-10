#pragma once

#include <memory>
#include <string>

#include "Enemy.h"

class EnemyFactory {
public:
	static std::unique_ptr<Enemy> Create(const std::string& type, int x, int y);
};