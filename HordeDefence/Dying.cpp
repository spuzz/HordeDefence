#include "Dying.h"
#include "Attack.h"
#include "MoveTo.h"



Dying::Dying(Unit*  nActor) : Action(nActor)
{
	mActor->getAnimation()->setAnimation("death", 0.2);
	mDeathTimer = 0.2;
}

bool Dying::update(float seconds)
{
	if (mDeathTimer > 0)
	{
		mDeathTimer -= seconds;

	}
	else
	{
		mActor->setDead(true);
	}
	return false;
}

Dying::~Dying()
{
}
