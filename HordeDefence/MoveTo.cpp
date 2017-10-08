#include "MoveTo.h"



MoveTo::MoveTo(Unit*  nActor, const Vector3D& nTarget) : Action(nActor)
{
	mActor->getAnimation()->setAnimation("move", 0.5);
	mActor->setTarget(nTarget);
}



bool MoveTo::update(float seconds)
{
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
