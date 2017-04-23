#pragma once
#include <string>
#include "Animation.h"
using std::string;
class UnitType
{
public:
	UnitType();
	virtual ~UnitType();

	void setName(string nName) { mName = nName; }
	void setMoveSpeed(float nMoveSpeed) { mMoveSpeed = nMoveSpeed; }
	void setAttackSpeed(float nAttackSpeed) { mAttackSpeed = nAttackSpeed; }
	void setSize(float nSize) { mSize = nSize; }
	void setRange(float nRange) { mRange = nRange; }
	void setHitPoints(float nHitPoints) { mHitPoints = nHitPoints; }
	void setAnimation(Animation nAnim) { mAnim = nAnim; }

	string getName() const { return mName; }
	float getMoveSpeed() const { return mMoveSpeed; }
	float getAttackSpeed() const { return mAttackSpeed; }
	float getSize() const { return mSize; }
	float getRange() const { return mRange; }
	float getHitPoints() const { return mHitPoints; }
	Animation getAnimation() const { return mAnim; }
private:
	string mName;
	float mMoveSpeed;
	float mAttackSpeed;
	float mSize;
	float mHitPoints;
	float mRange;
	Animation mAnim;

};

