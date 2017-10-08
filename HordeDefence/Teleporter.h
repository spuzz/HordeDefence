#pragma once
#include "Unit.h"
class Teleporter :
	public Unit
{
public:
	Teleporter(shared_ptr<gridVector> inAstarMap, Vector3D inLocation, UnitType type, std::map<string,ProjectileType> nProjTypes, const int& nGameObjectID);
	virtual ~Teleporter();

	void draw(const float& xScreenLoc, const float& yScreenLoc, const float& zScreenLoc, textureLoader& txtrLoader);
	virtual void hit(const float& nDamage, Unit* nHitBy);
	virtual std::vector<GLuint> GetTextures(textureLoader& txtrLoader);
	virtual void attack(shared_ptr<Unit> enemy);
};

