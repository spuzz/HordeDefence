#pragma once

#include <vector>
#include "Buff.h"
#include "Debuff.h"
#include "Weapon.h"
#include "Armor.h"
#include "Offhand.h"
#include "GameMath.h"
#include "Animation.h"
#include <memory>
#include "AstarService.h"
#include "GameObject.h"
#include "UnitType.h"
#include "Action.h"
#include "Idle.h"

enum direction { WEST, NORTHWEST, NORTH, NORTHEAST, EAST, SOUTHEAST, SOUTH, SOUTHWEST };
enum statuses { IDLE, ATTACK, DEFEND, MOVE, ATTACKMOVE };
using std::pair;
using std::string;

class Collision;
class Unit : public GameObject
{
public:

	Unit(shared_ptr<gridVector> inAstarMap, Vector3D inLocation, UnitType type, const int& nGameObjectID);
	virtual ~Unit();

	// Unit Info functions
	void setDirection(direction inDirection) { mFacing = inDirection; }
	void setMovement(float inMovement) { mMovement = inMovement;  }
	void setSize(float inSize) { mSize = inSize; }
	void setRange(float inRange) { mRange = inRange; }
	void setMaxHealth(float inMaxHealth) { mMaxHealth = inMaxHealth;  }
	void setCurrentHealth(float inCurrentHealth) { mCurrentHealth = inCurrentHealth;  }
	void setGender(bool inGender) { mGender = inGender; }
	void setAttackSpeed(float inAttackSpeed) { mAttackSpeedPerSecond = inAttackSpeed; }
	void setRace(string inRace) { mRace = inRace; }
	void setClassType(string inType) { mClassType = inType; }
	void setPath(locationVector inPath) { mPath = inPath; }
	void setPlayer(const int& nPlayer) { mPlayer = nPlayer; }

	const direction getDirection() const { return mFacing; }
	const string getClassType() const { return mClassType; }
	const string getRace() const { return mRace;  }
	const float getMovement() const { return mMovement; }
	const float getSize() const { return mSize; }
	const float getRange() const { return mRange; }
	const float getCurrentHealth() const { return mCurrentHealth; }
	const float getMaxHealth() const { return mMaxHealth; }
	const bool getGender() const { return mGender; }
	const float getAttackSpeed() const { return mAttackSpeedPerSecond;  }
	locationVector getPath()  { return mPath; }
	locationVector& getChangeablePath() { return mPath; }
	void setAnimation(Animation inAnim) { mAnimation = std::make_shared<Animation>(inAnim); }
	std::shared_ptr<Animation> getAnimation() const { return mAnimation; }
	GameMath::Circle getBoundingBox() { return mBoundingBox;  }
	int getPlayer() { return mPlayer;  }
	

	// Update functions
	virtual void attack();
	virtual void attackMove(GameMath::Vector3D inTarget);
	virtual void update(float nSeconds);
	virtual void move(Vector3D nMoveVec);
	virtual void dying();

	virtual void draw(const float& xScreenLoc, const float& yScreenLoc, const float& zScreenLoc, textureLoader& txtrLoader);
	virtual std::vector<GLuint> GetTextures(textureLoader& txtrLoader);
	Vector3D getTarget() { return mTarget; }
	shared_ptr<Unit> getTargetUnit() { return mTargetUnit; }

	void setTarget(GameMath::Vector3D inTarget){ mTarget = inTarget; }
	void newTarget(GameMath::Vector3D inTarget);
	void newTarget(shared_ptr<Unit> nUnit);
	void clearTargetUnit() { mTargetUnit = nullptr; }
	void findPath();
	virtual Vector3D findDirection();

	const bool getStatusChanged() const { return mStatusChanged; }
	void setStatusChanged(bool inStatusChanged) { mStatusChanged = inStatusChanged; }
	const statuses getStatus() const { return mStatus; }
	void setStatus(statuses inStatus);
	bool getAttackMove() { return mAttackMove;  }
	void setAttackMove(bool nAttackMove) {  mAttackMove = nAttackMove; }

	void setSelected(const bool& inSelected) { mSelected = inSelected; }
	void setAction(shared_ptr<Action> nAction) { mAction = shared_ptr<Action>(nAction);  }

	bool collide(const GameMath::Circle& circle);
	bool collide(const GameMath::Rectangle& rect);
	bool collide(const GameMath::Vector3D& point);

	shared_ptr<Unit> getNearestEnemyUnit(const int& nRange);
	virtual void hit(const float& nDamage,Unit* nHitBy);
	virtual void damage(const float& nDamage);
	virtual void heal(const float& nHeal);
	// Equipment function
	const Weapon getEquippedWeapon() { return mEquippedWeapon; }
	const Armor getEquippedArmor() { return mEquippedArmor; }
	const Offhand getEquippedOffhand() { return mEquippedOffhand; }
	void equipWeapon(const Weapon &inWeapon);
	void equipArmor(const Armor &armor);
	void equipOffhand(const Offhand &offhand);

	void SetEquipment(const string& weapon, const string& armor, const string& offhand);

	// Settings to allow changing equipment in UI
	bool canUseEquipment() { return mCanUseEquipment; }

	// Pathfinding function
	void setAstarGrid(shared_ptr<gridVector> inAstarGrid) { mAstarGrid = inAstarGrid; }
	shared_ptr<gridVector> getAstarGrid() { return mAstarGrid; }
	void setCollisionSystem(shared_ptr<Collision> nCol) { mCollisionSystem = nCol;  }

protected:
	virtual void changeAnim(string anim);
	virtual bool checkReachedTarget(pair<float,float> location,pair<float,float> target);
	virtual void setLookDirection();
	GameMath::Vector3D mTarget;
	shared_ptr<Unit> mTargetUnit;
	GameMath::Circle mBoundingBox;
	GameMath::Vector3D mDirection;
	int mXDirMod, mYDirMod;
	shared_ptr<AstarService> astar;

	// Unit Info variables
	float mMovement;
	float mAttackSpeedPerSecond;
	float mMaxHealth;
	float mCurrentHealth;
	float mSize;
	float mRange;
	bool mGender;
	string mRace;
	string mClassType;

	// Player variable
	// 0 = Human  Player
	int mPlayer;

	// Update variables
	bool mSelected;
	bool mStatusChanged;

	// Set to true to auto attack units when moving through path
	bool mAttackMove;

	std::shared_ptr<Animation> mAnimation;
	statuses mStatus;
	direction mFacing;
	std::shared_ptr<Action> mAction;
	
	// Equipment variables
	std::vector<Buff> mBuffs;
	std::vector<Debuff> mDebuffs;

	Weapon mEquippedWeapon;
	Armor mEquippedArmor;
	Offhand mEquippedOffhand;

	bool mCanUseEquipment;

	// Path Finding variables
	locationVector mPath;
	shared_ptr<gridVector> mAstarGrid;
	shared_ptr<Collision> mCollisionSystem;
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
