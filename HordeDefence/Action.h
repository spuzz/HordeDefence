#pragma once


class Unit;
class GameObject;
class Action
{
public:
	Action(Unit* nActor, GameObject*  nTarget = nullptr);
	virtual ~Action();

	virtual void reset() {};
	virtual bool update(float seconds) = 0;
protected:
	Unit* mActor;
	GameObject*  mTarget;
};

