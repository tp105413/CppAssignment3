#pragma once

class Entity {
protected:
	int x;
	int y;
	int hp;
	char symbol;

public:
	Entity(int x, int y, int hp, char symbol);
	virtual ~Entity() = default;

	int GetX() const;
	int GetY() const;
	int GetHp() const;
	char GetSymbol() const;
	bool IsAlive() const;

	void Move(int dX, int dY);
	void TakeDamage(int amount);
	void SetPosition(int newX, int newY);
};