#pragma once
#include "Action.h"
#include "MoveToInRange.h"

class Attack :
	public Action
{
public:
	Attack(Unit*  nActor, shared_ptr<Unit> nTarget);
	virtual ~Attack();

	bool update(float seconds);
	bool mAttacking;
	float mAttackCD;
	MoveToInRange* mMoveToTarget;
};

