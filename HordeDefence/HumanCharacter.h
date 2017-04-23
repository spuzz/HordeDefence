#pragma once
#include "Unit.h"
class HumanCharacter :
	public Unit
{
public:
	HumanCharacter(shared_ptr<gridVector> inAstarMap, Vector3D inLocation, UnitType type, const int& nGameObjectID);
	virtual ~HumanCharacter();

	virtual std::vector<GLuint> GetTextures(textureLoader& txtrLoader);
};

