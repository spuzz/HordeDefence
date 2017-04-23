#include "MoveToUnit.h"


MoveToUnit::MoveToUnit(Unit*  nActor, GameObject* nTarget) : Action(nActor, nTarget)
{
	mActor->getAnimation()->setAnimation("move", 0.5);
}


bool MoveToUnit::update(float seconds)
{
	Vector3D moveVec(mActor->getIsoLocation() + (mActor->findDirection()*mActor->getMovement()*seconds));
	moveVec.z = 0;
	if (GameMath::GameMath::circleToCircle(GameMath::Circle(moveVec, mActor->getBoundingBox().mRadius), mActor->getTargetUnit()->getBoundingBox()))
	{
		return true;
	}
	else
	{
		mActor->move(moveVec);
	}
	return false;
}

MoveToUnit::~MoveToUnit()
{
}
