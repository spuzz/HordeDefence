#include "Action.h"
#include "Idle.h"
#include "Unit.h"


Action::Action(Unit* nActor) : mActor(nActor)
{
	mLocked = false;
}


Action::~Action()
{
}
