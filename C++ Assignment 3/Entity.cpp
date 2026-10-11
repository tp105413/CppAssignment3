#include "Entity.h"

Entity::Entity(int x, int y, int hp, char symbol) {
	this->x = x;
	this->y = y;
	this->hp = hp;
	this->symbol = symbol;
}

int Entity::GetX() const {
	return x;
}

int Entity::GetY() const {
	return y;
}

int Entity::GetHp() const{
	return hp;
}

char Entity::GetSymbol() const {
	return symbol;
}

bool Entity::IsAlive() const {
	return hp > 0;
}

void Entity::Move(int dX, int dY) {
	x += dX;
	y += dY;
}

void Entity::TakeDamage(int amount) {
	hp -= amount;
	if (hp < 0) {
		hp = 0;
	}
}

void Entity::Heal(int amount) {
	hp += amount;
}

void Entity::SetPosition(int newX, int newY) {
	x = newX;
	y = newY;
}