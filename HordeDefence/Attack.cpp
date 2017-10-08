#include "Attack.h"
#include "Unit.h"


Attack::Attack(Unit*  nActor, shared_ptr<Unit> nTarget) : Action(nActor)
{
	mActor->setTargetUnit(nTarget);
	mMoveToTarget = new MoveToInRange(mActor,nTarget);
	mAttacking = false;
	mAttackCD = 0.3;
}

bool Attack::update(float seconds)
{
	if (mAttacking)
	{
		
		if (mAttackCD > 0)
		{
			mAttackCD -= seconds;
			return false;
		}
		else
		{
			mLocked = false;
			mAttackCD = 0.3;
			mAttacking = false;
			mActor->getAnimation()->setAnimation("idle", 0.5);
		}
	}

	if (mActor->getTargetUnit() == nullptr)
	{
		return true;
	}
	if (!mActor->getTargetUnit()->isTargetable())
	{
		return true;
	}
	if (mAttacking == false && mMoveToTarget->update(seconds))
	{
		mActor->attack(mActor->getTargetUnit());
		mAttacking = true;
	}
	return false;
}


Attack::~Attack()
{
}
