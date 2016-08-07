#pragma once

#include <vector>
#include "Buff.h"
#include "Debuff.h"
#include "Weapon.h"
#include "Armor.h"
#include "Offhand.h"
#include "Point.h"
#include "Animation.h"
#include <memory>
#include "AstarService.h"
#include "Sprite.h"

enum direction { WEST, NORTHWEST, NORTH, NORTHEAST, EAST, SOUTHEAST, SOUTH, SOUTHWEST };
enum statuses { IDLE, ATTACK, DEFEND, MOVE, ATTACKMOVE };
using std::pair;
using std::string;
class Unit : public Sprite
{
public:

	Unit(shared_ptr<gridVector> inAstarMap,Vect inLocation);

	virtual void draw(const float& xScreenLoc, const float& yScreenLoc, const float& zScreenLoc, textureLoader& txtrLoader);
	const Weapon getEquippedWeapon() { return mEquippedWeapon; }
	const Armor getEquippedArmor() { return mEquippedArmor; }
	const Offhand getEquippedOffhand() { return mEquippedOffhand; }
	void equipWeapon(const Weapon &inWeapon);
	void equipArmor(const Armor &armor);
	void equipOffhand(const Offhand &offhand);

	void addWeapon(const Weapon &inWeapon);
	void addArmor(const Armor &inArmor);
	void addOffhand(const Offhand &inOffhand);
	const std::vector<Weapon> getWeaponsStock() const { return mWeapons; }
	const std::vector<Armor> getArmorStock() const { return mArmor; }
	const std::vector<Offhand> getoffhandStock() const { return mOffhands; }

	void setAvailableWeapons(std::vector<string> inWeapons) { mAvailableWeaponList = inWeapons; }
	void setAvailableArmor(std::vector<string> inArmor) { mAvailableArmorList = inArmor; }
	void setAvailableOffhand(std::vector<string> inOffhand) { mAvailableOffhandList = inOffhand; }
	std::vector<string> getAvailableWeapon() {return mAvailableWeaponList;}
	std::vector<string> getAvailableArmor() { return mAvailableArmorList; }
	std::vector<string> getAvailableOffhand() { return mAvailableOffhandList; }



	void setDirection(direction inDirection) { mFacing = inDirection; }
	void setMovement(float inMovement) { mMovement = inMovement;  }
	void setSize(float inSize) { mSize = inSize; }
	void setMaxHealth(float inMaxHealth) { mMaxHealth = inMaxHealth;  }
	void setCurrentHealth(float inCurrentHealth) { mCurrentHealth = inCurrentHealth;  }
	void setGender(bool inGender) { mGender = inGender; }
	void setAttackSpeed(float inAttackSpeed) { mAttackSpeedPerSecond = inAttackSpeed; }
	void setRace(string inRace) { mRace = inRace; }
	void setClassType(string inType) { mClassType = inType; }
	void setPath(locationVector inPath) { mPath = inPath; }


	const direction getDirection() const { return mFacing; }
	const string getClassType() const { return mClassType; }
	const string getRace() const { return mRace;  }
	const float getMovement() const { return mMovement; }
	const float getSize() const { return mSize; }
	const float getCurrentHealth() const { return mCurrentHealth; }
	const float getMaxHealth() const { return mMaxHealth; }
	const bool getGender() const { return mGender; }
	const float getAttackSpeed() const { return mAttackSpeedPerSecond;  }
	locationVector getPath()  { return mPath; }
	locationVector& getChangeablePath() { return mPath; }
	void setAnimation(Animation inAnim) { mAnimation = std::make_shared<Animation>(inAnim); }
	std::shared_ptr<Animation> getAnimation() const { return mAnimation; }
	virtual void attack() = 0;
	virtual void update() = 0;
	virtual void move() = 0;
	virtual void dying() = 0;

	void setAstarGrid(shared_ptr<gridVector> inAstarGrid) { mAstarGrid = inAstarGrid; }
	shared_ptr<gridVector> getAstarGrid() { return mAstarGrid; }
	void setTarget(std::pair<float, float> inTarget){ mTarget = inTarget; }
	void newTarget(std::pair<float, float> inTarget);
	virtual void findDirection();

	const bool getStatusChanged() const { return mStatusChanged; }
	void setStatusChanged(bool inStatusChanged) { mStatusChanged = inStatusChanged; }
	const statuses getStatus() const { return mStatus; }
	void setStatus(statuses inStatus);
	virtual ~Unit();

protected:
	virtual void changeAnim(string anim);
	virtual bool checkReachedTarget(pair<float,float> location,pair<float,float> target);
	pair<float, float> mTarget;
	int mXDirMod, mYDirMod;
	shared_ptr<AstarService> astar;
private:
	float mMovement;
	float mAttackSpeedPerSecond;
	float mMaxHealth;
	float mCurrentHealth;
	float mSize;

	

	std::shared_ptr<Animation> mAnimation;
	bool mGender;
	string mRace;
	string mClassType;
	direction mFacing;
	
	bool mStatusChanged;
	std::vector<string> mAvailableWeaponList;
	std::vector<string> mAvailableArmorList;
	std::vector<string> mAvailableOffhandList;

	std::vector<Buff> mBuffs;
	std::vector<Debuff> mDebuffs;
	std::vector<Weapon> mWeapons;
	std::vector<Armor> mArmor;
	std::vector<Offhand> mOffhands;

	Weapon mEquippedWeapon;
	Armor mEquippedArmor;
	Offhand mEquippedOffhand;

	statuses mStatus;

	locationVector mPath;
	shared_ptr<gridVector> mAstarGrid;
};

class AstarHandler : public AstarLifeCycleHandler
{
public:
	AstarHandler(locationVector &path) : result(path) {};
	locationVector& result;

	void successOnPathFinding(locationVector path) {
		result = path;
	}

	void errorOnPathFinding() {

	}

};
