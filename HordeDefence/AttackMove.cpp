#include "AttackMove.h"



AttackMove::AttackMove(Unit*  nActor, const Vector3D& nTarget) : Action(nActor, nullptr)
{
	mActor->getAnimation()->setAnimation("move", 0.5);
	mTargetLoc = nTarget;
}



bool AttackMove::update(float seconds)
{
	if (mActor->getTargetUnit() != nullptr)
	{
		return true;
	}
	shared_ptr<Unit> unit = mActor->getNearestEnemyUnit(8);
	if (unit != nullptr)
	{
		mActor->newTarget(unit);
		mActor->setAction(shared_ptr<Action>(new Attack(mActor, mActor->getTargetUnit().get())));
	}
	else
	{
		Vector3D moveVec(mActor->getIsoLocation() + (mActor->findDirection()*mActor->getMovement()*seconds));
		mActor->move(moveVec);
		if (GameMath::GameMath::pointToCircle(mActor->getTarget(), mActor->getBoundingBox()))
		{
			return true;
		}
		
	}
	return false;
}

AttackMove::~AttackMove()
{
}
