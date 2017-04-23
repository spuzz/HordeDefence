#pragma once
#include "Action.h"
class Idle :
	public Action
{
public:
	Idle(Unit*  nActor, GameObject*   nTarget);
	virtual ~Idle();

	bool update(float seconds);

};

