#pragma once
#include "Action.h"
#include "MoveToUnit.h"
class Attack :
	public Action
{
public:
	Attack(Unit*  nActor, GameObject*   nTarget);
	virtual ~Attack();

	bool update(float seconds);
	bool mAttacking;
	float mAttackCD;
	MoveToUnit mMoveToTarget;
};

