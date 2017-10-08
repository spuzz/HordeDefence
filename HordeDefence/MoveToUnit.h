#pragma once
#include "Action.h"

class MoveToUnit :
	public Action
{
public:
	MoveToUnit(Unit*  nActor, shared_ptr<Unit> nTarget);
	virtual ~MoveToUnit();

	bool update(float seconds);

private:
};

