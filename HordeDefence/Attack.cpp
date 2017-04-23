#include "Attack.h"



Attack::Attack(Unit*  nActor, GameObject*   nTarget) : Action(nActor, nTarget), mMoveToTarget(nActor, nTarget)
{
	mAttacking = false;
	mAttackCD = 0.8;
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
			mAttackCD = 0.8;
			mAttacking = false;
			mActor->getAnimation()->setAnimation("move", 0.5);
		}
	}

	if (mActor->getTargetUnit() == nullptr)
	{
		return true;
	}
	if (!mActor->getTargetUnit()->isTargetable())
	{
		mActor->clearTargetUnit();
		return true;
	}
	if (mAttacking == false && mMoveToTarget.update(seconds))
	{
		mActor->getAnimation()->setAnimation("attack", 0.8);
		mActor->getTargetUnit()->hit(30, mActor);
		mAttacking = true;
	}

	

	return false;
}

Attack::~Attack()
{
}
