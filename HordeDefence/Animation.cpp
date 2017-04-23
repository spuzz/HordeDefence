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
		mAnimLength = mAnimations[mCurrentAnimation].second;
		if (inSpeed != -1) {
			setSpeed(inSpeed);
		}
	}
}

void Animation::update(const float& nSeconds)
{

	if (updatable == true) {
		mCurrentAnimFrame += nSeconds*(mAnimLength / mAnimSpeed);
		if (mCurrentAnimFrame >= (mAnimations[mCurrentAnimation].first + mAnimations[mCurrentAnimation].second)) {
			mCurrentAnimFrame = mAnimations[mCurrentAnimation].first;
		}
	}

}

Animation::~Animation()
{
}
