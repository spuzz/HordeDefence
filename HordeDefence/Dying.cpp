#include "Dying.h"
#include "Attack.h"
#include "MoveTo.h"



Dying::Dying(Unit*  nActor) : Action(nActor, nullptr)
{
	mActor->getAnimation()->setAnimation("death", 1.5);
	mDeathTimer = 1.5;
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
