#include "Action.h"
#include "Idle.h"
#include "Unit.h"


Action::Action(Unit* nActor, GameObject*  nTarget) : mActor(nActor), mTarget(nTarget)
{
}


Action::~Action()
{
}
