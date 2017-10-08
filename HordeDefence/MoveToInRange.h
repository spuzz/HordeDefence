#pragma once
#include "Action.h"

class MoveToInRange :
	public Action
{
public:
	MoveToInRange(Unit*  nActor, std::shared_ptr<Unit> nTarget);
	virtual ~MoveToInRange();

	bool update(float seconds);

};

