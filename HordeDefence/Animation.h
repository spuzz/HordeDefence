#pragma once
#include <iostream>
#include <map>


using std::string;
class Animation
{
public:
	Animation();
	virtual ~Animation();

	

	virtual void update();
	virtual void addAnimation(string name, int initialFrame, int frames) { mAnimations[name] = std::pair<int,int>(initialFrame,frames); }
	virtual void removeAnimation(string name) { mAnimations.erase(mAnimations.find(name)); }

	void setSpeed(float inAnimSpeed) { mAnimSpeed = inAnimSpeed; }
	const float getAnimSpeed() const { return mAnimSpeed; }
	void setCurrentAnimFrame(float inAnim) { mCurrentAnimFrame = inAnim; }
	void nextFrame() { mCurrentAnimFrame++; }
	const float getCurrentAnimFrame() { return mCurrentAnimFrame; }
	void setAnimation(string inName, float inSpeed = -1);
private:
	std::map < string, std::pair<int, int>> mAnimations;
	float mCurrentAnimFrame;
	string mCurrentAnimation;
	float mAnimSpeed;
	bool updatable;
};

