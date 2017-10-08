#include "MoveToInRange.h"
#include "Unit.h"



MoveToInRange::MoveToInRange(Unit*  nActor, shared_ptr<Unit> nTarget) : Action(nActor)
{
	mActor->setTargetUnit(nTarget);
	mActor->getAnimation()->setAnimation("move", 0.5);
}


bool MoveToInRange::update(float seconds)
{
	// Reset target lock to find new path if it has moved
	if (int(mActor->getTargetUnit()->getIsoLocation().x) != int(mActor->getTarget().x) || int(mActor->getTargetUnit()->getIsoLocation().y) != int(mActor->getTarget().y))
	{
		mActor->setTargetUnit(mActor->getTargetUnit());
	}
	float distVec = (mActor->getIsoLocation() - mActor->getTargetUnit()->getIsoLocation()).GetMagnitude() - mActor->getTargetUnit()->getBoundingBox().mRadius - mActor->getBoundingBox().mRadius;
	if (mActor->getRange() >= distVec)
	{
		return true;
	}
	else
	{
		Vector3D moveVec(mActor->getIsoLocation() + (mActor->findDirection()*mActor->getMovement()*seconds));
		mActor->move(moveVec);
	}
	return false;
}

MoveToInRange::~MoveToInRange()
{
}
