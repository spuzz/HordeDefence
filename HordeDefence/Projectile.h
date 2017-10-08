#pragma once
#include <string>
#include "textureLoader.h"
#include "ProjectileType.h"
#include "Vector3D.h"
#include <memory>
using std::string;

class Unit;
class Projectile
{
public:

	Projectile(GameMath::Vector3D nLoc, std::shared_ptr<Unit> nTargetUnit, float nAttackDamage, ProjectileType nType, int nPlayer);
	Projectile(GameMath::Vector3D nLoc, GameMath::Vector3D nTargetLoc, bool nDmgOrHeal, float nAttackDamage, ProjectileType nType, int nPlayer);
	virtual ~Projectile();

	void Draw(const float& xScreenLoc, const float& yScreenLoc, const float& zScreenLoc, textureLoader& txtrLoader);
	bool Update(float seconds);

	std::shared_ptr<Unit> getTargetUnit() { return mTargetUnit;  }
	GameMath::Vector3D getTarget() { return mTargetLoc; }

protected:
	int getLookDirection();

	
	std::shared_ptr<Unit> mTargetUnit;
	GameMath::Vector3D mTargetLoc;
	string mAttackName;
	string mAttackType;
	float mProjectileSpeed;
	int mTotalFrames;
	float mAttackDamage;
	GameMath::Vector3D mLocation;
	GameMath::Vector3D mDirection;
	Animation mAnim;
	int mPlayer; 
	bool mDmgOrHeal;
};

