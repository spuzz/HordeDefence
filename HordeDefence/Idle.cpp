#include "Idle.h"
#include "Attack.h"
#include "MoveTo.h"
#include "AttackMove.h"

Idle::Idle(Unit*  nActor, GameObject*   nTarget) : Action(nActor, nTarget)
{
	mActor->getAnimation()->setAnimation("idle", 1);
}


bool Idle::update(float seconds)
{
	if (mActor->getTargetUnit() != nullptr)
	{
		mActor->setAction(shared_ptr<Action>(new Attack(mActor, mActor->getTargetUnit().get())));
	}
	else if (!mActor->collide(mActor->getTarget()))
	{
		if (mActor->getAttackMove())
		{
			mActor->setAction(shared_ptr<Action>(new AttackMove(mActor, mActor->getTarget())));
		}
		else
		{
			mActor->setAction(shared_ptr<Action>(new MoveTo(mActor, mActor->getTarget())));
		}
		
	}
	else
	{
		shared_ptr<Unit> unit = mActor->getNearestEnemyUnit(8);
		if (unit != nullptr)
		{
			mActor->newTarget(unit);
			mActor->setAction(shared_ptr<Action>(new Attack(mActor, mActor->getTargetUnit().get())));
		}
	}

	return false;
}

Idle::~Idle()
{
}
