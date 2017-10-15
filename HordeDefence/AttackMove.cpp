#include "AttackMove.h"
#include "Unit.h"


AttackMove::AttackMove(Unit*  nActor, const Vector3D& nTarget) : Action(nActor)
{
	mActor->getAnimation()->setAnimation("move", 0.5);
	mMoveTo = new MoveTo(mActor, nTarget);
	mTargetLoc = nTarget;
	changedTarget = false;
	mAttack = nullptr;
	attackMoveCD = 0;
}



bool AttackMove::update(float seconds)
{

	shared_ptr<Unit> unit = mActor->getNearestEnemyUnit(8);
	if (unit != nullptr)
	{
		if (mActor->getTargetUnit() == nullptr ||  mActor->getTargetUnit()->getObjectID() != unit->getObjectID() && attackMoveCD <= 0)
		{
			changedTarget = true;
			mAttack = new Attack(mActor, unit);
			attackMoveCD = 0.5;
		}
		mAttack->update(seconds);
	}
	else
	{
		if (changedTarget == true)
		{
			mMoveTo = new MoveTo(mActor,mTargetLoc);
			changedTarget = false;
		}
		if (mMoveTo->update(seconds) == true)
		{
			return true;
		}
	}
	if (attackMoveCD >= 0)
	{
		attackMoveCD = attackMoveCD - seconds;
	}

	return false;
}

AttackMove::~AttackMove()
{
}
