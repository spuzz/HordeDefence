#include "MoveToUnit.h"
#include "Unit.h"

MoveToUnit::MoveToUnit(Unit*  nActor, shared_ptr<Unit> nTarget) : Action(nActor)
{
	mActor->setTargetUnit(nTarget);
	mActor->getAnimation()->setAnimation("move", 0.5);
}


bool MoveToUnit::update(float seconds)
{
	// Reset target lock to find new path if it has moved
	if (int(mActor->getTargetUnit()->getIsoLocation().x) != int(mActor->getTarget().x) || int(mActor->getTargetUnit()->getIsoLocation().y) != int(mActor->getTarget().y))
	{
		mActor->setTargetUnit(mActor->getTargetUnit());
	}
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
