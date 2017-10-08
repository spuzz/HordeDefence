#include "Idle.h"
#include "Attack.h"
#include "MoveTo.h"
#include "AttackMove.h"

Idle::Idle(Unit*  nActor) : Action(nActor)
{
	mActor->getAnimation()->setAnimation("idle", 1);
}


bool Idle::update(float seconds)
{
	//if (mActor->getTargetUnit() != nullptr)
	//{
	//	mActor->setAction(shared_ptr<Action>(new Attack(mActor)));
	//}
	//else if (!mActor->collide(mActor->getTarget()))
	//{
	//	if (mActor->getAttackMove())
	//	{
	//		mActor->setAction(shared_ptr<Action>(new AttackMove(mActor, mActor->getTarget())));
	//	}
	//	else
	//	{
	//		mActor->setAction(shared_ptr<Action>(new MoveTo(mActor, mActor->getTarget())));
	//	}
	//	
	//}
	//else
	//{
	shared_ptr<Unit> unit = mActor->getNearestEnemyUnit(8);
	if (unit != nullptr)
	{
		if (mActor->getTargetUnit() == nullptr || unit->getObjectID() != mActor->getTargetUnit()->getObjectID())
		{
			mAttack = new Attack(mActor,unit);
		}
		
		mAttack->update(seconds);
	}
	else
	{
		mActor->setTarget(mActor->getIsoLocation());
		mActor->getAnimation()->setAnimation("idle", 0.5);
	}
	//}

	return false;
}

Idle::~Idle()
{
}
