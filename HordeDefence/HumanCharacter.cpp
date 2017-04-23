#include "HumanCharacter.h"



HumanCharacter::HumanCharacter(shared_ptr<gridVector> inAstarMap, Vector3D inLocation, UnitType type, const int& nGameObjectID) : Unit(inAstarMap,inLocation,type,nGameObjectID)
{
	mCanUseEquipment = true;
}


HumanCharacter::~HumanCharacter()
{
}

std::vector<GLuint> HumanCharacter::GetTextures(textureLoader& txtrLoader)
{
	return txtrLoader.retrieveUnitTexture(getRace(), getGender(), getClassType(), getEquippedWeapon().getType(), getEquippedArmor().getType(), getEquippedOffhand().getType());
}

