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
	void setClass(string nClass) { mClass = nClass; }
	void setMoveSpeed(float nMoveSpeed) { mMoveSpeed = nMoveSpeed; }
	void setAttackSpeed(float nAttackSpeed) { mAttackSpeed = nAttackSpeed; }
	void setSize(float nSize) { mSize = nSize; }
	void setRange(float nRange) { mRange = nRange; }
	void setHitPoints(float nHitPoints) { mHitPoints = nHitPoints; }
	void setAnimation(Animation nAnim) { mAnim = nAnim; }
	void setUnitInfoTexture(string nUnitInfoTexPath) { mUnitInfoTexPath = nUnitInfoTexPath; }

	string getName() const { return mName; }
	string getClass() const { return mClass; }
	float getMoveSpeed() const { return mMoveSpeed; }
	float getAttackSpeed() const { return mAttackSpeed; }
	float getSize() const { return mSize; }
	float getRange() const { return mRange; }
	float getHitPoints() const { return mHitPoints; }
	Animation getAnimation() const { return mAnim; }
	string getUnitInfoTexPath() const{ return mUnitInfoTexPath; }
private:
	string mName;
	string mClass;
	float mMoveSpeed;
	float mAttackSpeed;
	float mSize;
	float mHitPoints;
	float mRange;
	Animation mAnim;
	string mUnitInfoTexPath;

};

