#pragma once
#include "Unit.h"
class Knight :
	public Unit
{
public:
	Knight(shared_ptr<gridVector> inAstarMap, float inMovement = -1.0f, float maxHealth = -1.0f, float inCurrentHealth = -1.0f, std::pair<float, float> inLocation = std::pair<float, float>(-1.0f, -1.0f),
		bool inGender = 0);
	

	virtual void attack();
	virtual void update();
	virtual void move();
	virtual void dying();
	virtual ~Knight();

protected:
	std::vector<std::vector<int>> gridVectorTest;
	
	
};

