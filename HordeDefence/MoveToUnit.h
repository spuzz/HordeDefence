#pragma once
#include "Action.h"
#include "Unit.h"
class MoveToUnit :
	public Action
{
public:
	MoveToUnit(Unit*  nActor, GameObject* nTarget);
	virtual ~MoveToUnit();

	bool update(float seconds);

private:
};

