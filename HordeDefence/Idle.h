#pragma once
#include "Action.h"
#include "Attack.h"

class Idle :
	public Action
{
public:
	Idle(Unit*  nActor);
	virtual ~Idle();

	bool update(float seconds);

private:
	Attack* mAttack;
};

