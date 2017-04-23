#include "MoveToInRange.h"




MoveToInRange::MoveToInRange(Unit*  nActor, GameObject* nTarget) : Action(nActor, nTarget)
{
	mActor->getAnimation()->setAnimation("move", 0.5);
}


bool MoveToInRange::update(float seconds)
{
	float distVec = (mActor->getIsoLocation() - mActor->getTargetUnit()->getIsoLocation()).GetMagnitude() - mActor->getTargetUnit()->getBoundingBox().mRadius;
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
