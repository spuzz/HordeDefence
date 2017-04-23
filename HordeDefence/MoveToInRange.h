#pragma once
#include "Action.h"
#include "Unit.h"
class MoveToInRange :
	public Action
{
public:
	MoveToInRange(Unit*  nActor, GameObject* nTarget);
	virtual ~MoveToInRange();

	bool update(float seconds);

};

