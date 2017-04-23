#pragma once
#include "Action.h"
class Dying :
	public Action
{
public:
	Dying(Unit*  nActor);
	virtual ~Dying();

	bool update(float seconds);

private:
	float mDeathTimer;
};

