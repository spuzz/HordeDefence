#pragma once
#include "Action.h"
#include "Attack.h"
#include "MoveTo.h"
class AttackMove :
	public Action
{
public:
	AttackMove(Unit*  nActor, const Vector3D& nTarget);
	virtual ~AttackMove();
	bool update(float seconds);

private:
	Vector3D mTargetLoc;
	Attack* mAttack;
	MoveTo* mMoveTo;
	bool changedTarget;
	float attackMoveCD;
};

