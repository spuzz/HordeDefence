#pragma once
#include "Action.h"
#include "Unit.h"
class MoveTo :
	public Action
{
public:
	MoveTo(Unit* nActor, const Vector3D&  nTarget);
	virtual ~MoveTo();

	bool update(float seconds);

private:
	Vector3D mTargetLoc;
};

