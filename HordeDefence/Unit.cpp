#include "Unit.h"
#include <algorithm>
#include "GridAstar.h"

Unit::Unit(shared_ptr<gridVector> inAstarMap) : mAnimation(new Animation()), mAstarGrid(inAstarMap)
{

}
void Unit::setStatus(statuses inStatus) 
{ 
	if (mStatus != inStatus) {
		setStatusChanged(true);
		mStatus = inStatus;
	}

}
void Unit::equipWeapon(const Weapon &inWeapon)
{
	if (std::find_if(mWeapons.begin(), mWeapons.end(), [=](const Weapon &w) { return w.getName() == inWeapon.getName(); }) != mWeapons.end())
	{
		mEquippedWeapon = inWeapon;
	}
	
}

void Unit::equipArmor(const Armor &inArmor)
{
	if (std::find_if(mArmor.begin(), mArmor.end(), [=](const Armor &a) { return a.getName() == inArmor.getName(); }) != mArmor.end())
	{
		mEquippedArmor = inArmor;
	}
	
}

void Unit::equipOffhand(const Offhand &inOffhand)
{
	if (std::find_if(mOffhands.begin(), mOffhands.end(), [=](const Offhand &o) { return o.getName() == inOffhand.getName(); }) != mOffhands.end())
	{
		mEquippedOffhand = inOffhand;
	}
}

void Unit::addWeapon(const Weapon &inWeapon)
{
	if (*mAvailableWeaponList.begin() == "All" || std::find_if(mAvailableWeaponList.begin(), mAvailableWeaponList.end(), [=](const string &w) { return w == inWeapon.getType(); }) != mAvailableWeaponList.end())
	{
		if (std::find_if(mWeapons.begin(), mWeapons.end(), [=](const Weapon &w) { return w.getName() == inWeapon.getName(); }) == mWeapons.end())
		{
			mWeapons.push_back(inWeapon);
		}
	}
}

void Unit::addArmor(const Armor &inArmor)
{
	if (*mAvailableArmorList.begin() == "All" || std::find_if(mAvailableArmorList.begin(), mAvailableArmorList.end(), [=](const string &a) { return a == inArmor.getType(); }) != mAvailableArmorList.end())
	{
		if (std::find_if(mArmor.begin(), mArmor.end(), [=](const Armor &a) { return a.getName() == inArmor.getName(); }) == mArmor.end())
		{
			mArmor.push_back(inArmor);
		}
	}
}

void Unit::addOffhand(const Offhand &inOffhand)
{
	if (*mAvailableOffhandList.begin() == "All" || std::find_if(mAvailableOffhandList.begin(), mAvailableOffhandList.end(), [=](const string &o) { return o == inOffhand.getType(); }) != mAvailableOffhandList.end())
	{
		if (std::find_if(mOffhands.begin(), mOffhands.end(), [=](const Offhand &o) { return o.getName() == inOffhand.getName(); }) == mOffhands.end())
		{
			mOffhands.push_back(inOffhand);
		}
	}
}

void Unit::changeAnim(string anim)
{
	
}
void Unit::newTarget(std::pair<float, float> inTarget)
{
	setTarget(inTarget);
	shared_ptr<AstarLifeCycleHandler> handler(new AstarHandler(getChangeablePath()));
	astar = shared_ptr<AstarService>(new GridAstarService(*getAstarGrid(), 10, -1));
	astar->submitPath(getLocation().first, getLocation().second, mTarget.first, mTarget.second, handler);
}

void Unit::findDirection()
{
	if (mPath.size() == 0) {
		return;
	}
	// Use top left of tile/unit location unless it is the final target
	pair<float, float> target(mPath[mPath.size() - 1].first - 0.5, mPath[mPath.size() - 1].second + 0.5);
	pair<float, float> location(mLocation.first - (mSize / 2), mLocation.second + (mSize / 2));
	float xMoveCheck = abs(target.first - location.first);
	float yMoveCheck = abs(target.second - location.second);

	if (xMoveCheck > mMovement) {
		if (target.first > location.first) {
			if (yMoveCheck > mMovement) {
				if (target.second < location.second)
				{
					mXDirMod = 1;
					mYDirMod = -1;
					mFacing = SOUTHEAST;
				}
				else
				{
					mXDirMod = 1;
					mYDirMod = 1;
					mFacing = NORTHEAST;
				}
			}
			else {
				mXDirMod = 1;
				mFacing = EAST;
			}
		}
		else {
			if (yMoveCheck > mMovement)
			{
				if (target.second < location.second)
				{
					mXDirMod = -1;
					mYDirMod = -1;
					mFacing = SOUTHWEST;
				}
				else
				{
					mXDirMod = -1;
					mYDirMod = 1;
					mFacing = NORTHWEST;
				}
			}
			else
			{
				mXDirMod = -1;
				mFacing = WEST;
			}
		}

	}
	else {
		if (yMoveCheck > mMovement)
		{
			if (target.second < location.second)
			{
				mYDirMod = -1;
				mFacing = SOUTH;
			}
			else
			{
				mYDirMod = 1;
				mFacing = NORTH;
			}
		}
		else {
			mXDirMod = mYDirMod = 0;
			mPath.erase(--mPath.end());
		}
	}
	if (checkReachedTarget(location, target) || (mXDirMod == 0 && mYDirMod == 0)) {

		

	}
	//if (target.first > location.first)
	//{
	//	if (target.second == location.second)
	//	{
	//		mXDirMod = 1;
	//		mFacing = EAST;
	//	}
	//	else if (target.second < location.second)
	//	{
	//		mXDirMod = 1;
	//		mYDirMod = -1;
	//		mFacing = SOUTHEAST;
	//	}
	//	else
	//	{
	//		mXDirMod = 1;
	//		mYDirMod = 1;
	//		mFacing = NORTHEAST;
	//	}
	//}
	//else if (target.first == location.first)
	//{
	//	if (target.second == location.second)
	//	{
	//		
	//		mFacing = SOUTH;
	//	}
	//	else if (target.second < location.second)
	//	{
	//		mYDirMod = -1;
	//		mFacing = SOUTH;
	//	}
	//	else
	//	{
	//		mYDirMod = 1;
	//		mFacing = NORTH;
	//	}
	//}
	//else if (target.first < location.first)
	//{
	//	if (target.second == location.second)
	//	{
	//		mXDirMod = -1;

	//		mFacing = WEST;
	//	}
	//	else if (target.second < location.second)
	//	{
	//		mXDirMod = -1;
	//		mYDirMod = -1;
	//		mFacing = SOUTHWEST;
	//	}
	//	else
	//	{
	//		mXDirMod = -1;
	//		mYDirMod = 1;
	//		mFacing = NORTHWEST;
	//	}
	//}


	//mFacing = SOUTH;
}

bool Unit::checkReachedTarget(pair<float, float> location, pair<float, float> target)
{
	pair<float, float> difference(target.first - location.first,target.second - location.second);
	pair<float, float> diffAfterMove(target.first - (location.first + (getMovement()*mXDirMod)), target.second - (location.second + (getMovement()*mYDirMod)));
	bool xReached = true;
	bool yReached = true;
	if ((difference.first < 0 && diffAfterMove.first < 0) || (difference.first > 0 && diffAfterMove.first > 0)) {
		xReached = false;
	}
	if ((difference.second < 0 && diffAfterMove.second < 0) || (difference.second > 0 && diffAfterMove.second > 0)) {
		yReached = false;
	}
	if (xReached && yReached) {
		return true;
	}
	else {
		return false;
	}
}

Unit::~Unit()
{

}
