#pragma once
#include <string>
#include "Animation.h"

using std::string;
class ProjectileType
{
public:
	ProjectileType();
	~ProjectileType();

	void setName(string nName) { mName = nName; }
	void setMoveSpeed(float nMoveSpeed) { mMoveSpeed = nMoveSpeed; }
	void setSize(float nSize) { mSize = nSize; }
	void setAnimation(Animation nAnim) { mAnim = nAnim; }
	void setTotalFrames(float nTotalFrames) { mTotalFrames = nTotalFrames; }
	void setAttackType(const string& nAttackType) { mAttackType = nAttackType; }
	void setAreaOfEffect(float nAreaOfEffect) { mAreaOfEffect = nAreaOfEffect; }

	string getName() const { return mName; }
	float getMoveSpeed() const { return mMoveSpeed; }
	float getSize() const { return mSize; }
	Animation getAnimation() const { return mAnim; }
	float getTotalFrames() const { return mTotalFrames; }
	string getAttackType() { return mAttackType; }
	float getAreaOfEffect() const { return mAreaOfEffect; }

private:
	string mName;
	float mMoveSpeed;
	float mSize;
	float mAreaOfEffect;
	float mTotalFrames;
	Animation mAnim;
	string mAttackType;
};


