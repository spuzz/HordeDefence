#include "MoveTo.h"



MoveTo::MoveTo(Unit*  nActor, const Vector3D& nTarget) : Action(nActor, nullptr)
{
	mActor->getAnimation()->setAnimation("move", 0.5);
	mTargetLoc = nTarget;
}



bool MoveTo::update(float seconds)
{
	if (mActor->getTargetUnit() != nullptr)
	{
		return true;
	}
	Vector3D moveVec(mActor->getIsoLocation() + (mActor->findDirection()*mActor->getMovement()*seconds));
	mActor->move(moveVec);
	if (GameMath::GameMath::pointToCircle(mActor->getTarget(), mActor->getBoundingBox()))
	{		
		return true;
	}
	return false;
}


MoveTo::~MoveTo()
{
}
