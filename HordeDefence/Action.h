#pragma once

#include <memory>
#include "Vector3D.h"

using std::shared_ptr;
using GameMath::Vector3D;

class Unit;
class GameObject;
class Action
{
public:
	Action(Unit* nActor);
	virtual ~Action();

	virtual void reset() {};
	virtual bool update(float seconds) = 0;
	Unit* getActor() { return mActor; }

	bool mLocked;
	
protected:
	Unit* mActor;
	
};

