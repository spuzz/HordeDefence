#pragma once
#include "Action.h"
#include "Unit.h"
#include "Attack.h"

class AttackMove :
	public Action
{
public:
	AttackMove(Unit*  nActor, const Vector3D& nTarget);
	virtual ~AttackMove();
	bool update(float seconds);

private:
	Vector3D mTargetLoc;
};

