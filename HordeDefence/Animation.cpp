#include "Animation.h"
#include <algorithm>

Animation::Animation()
{
}

void Animation::setAnimation(string inName, float inSpeed)
{
	updatable = true;
	if (mAnimations.find(inName) != mAnimations.end()) {
		setCurrentAnimFrame(mAnimations[inName].first);
		mCurrentAnimation = inName;
		if (inSpeed != -1) {
			setSpeed(inSpeed);
		}
	}
}

void Animation::update()
{

	if (updatable == true) {
		mCurrentAnimFrame += 1.0f*mAnimSpeed/5;
		if (mCurrentAnimFrame >= (mAnimations[mCurrentAnimation].first + mAnimations[mCurrentAnimation].second)) {
			mCurrentAnimFrame = mAnimations[mCurrentAnimation].first;
		}
	}

}

Animation::~Animation()
{
}
