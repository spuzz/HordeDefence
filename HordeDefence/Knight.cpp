#include "Knight.h"


Knight::Knight(shared_ptr<gridVector> inAstarMap,float inMovement, float inMaxHealth, float inCurrentHealth, Vector3D inLocation, bool inGender) : Unit(inAstarMap, inLocation,UnitType(),0)
{
	
	inMovement == -1 ? setMovement(1) : setMovement(inMovement);
	inMaxHealth == -1 ? setMaxHealth(100) : setMaxHealth(inMaxHealth);
	inCurrentHealth == -1 ? setCurrentHealth(100) : setCurrentHealth(inCurrentHealth);
	
	setTarget(Vector3D(-1, -1, 0));
	setGender(inGender);
	setAttackSpeed(0.5);
	setMovement(0.05);
	setSize(1);
	getAnimation()->addAnimation("idle", 0, 4);
	getAnimation()->addAnimation("attack", 12, 4);
	getAnimation()->addAnimation("move", 4, 8);
	getAnimation()->addAnimation("death", 15, 4);
	setRace("Human");
	setClassType("Knight");
	setAvailableWeapons(std::vector < string > {"All"});
	setAvailableArmor(std::vector < string > {"All"});
	setAvailableOffhand(std::vector < string > {"All"});
	Weapon wep("sword");
	Armor armor("steel");
	Offhand offhand("shield");
	addWeapon(wep);
	addArmor(armor);
	addOffhand(offhand);
	equipWeapon(wep);
	equipArmor(armor);
	equipOffhand(offhand);
	setStatus(IDLE);
	getAnimation()->setAnimation("idle", 0.3);
}


void Knight::attack()
{
	getAnimation()->setAnimation("attack",0.5);
}

void Knight::update()
{
	string animation;
	float animSpeed;
	if (getPath().size() != 0) {
		setStatus(MOVE);
	}
	else {
		setStatus(IDLE);
	}
	switch (getStatus()) {
	case IDLE:
		animation = "idle";
		animSpeed = 0.4;
		break;
	case MOVE:
		animation = "move";
		animSpeed = 0.5;
		move();
		break;
	case ATTACK:
		animation = "attack";
		animSpeed = 0.5;
		break;
	case ATTACKMOVE:
		animation = "move";
		animSpeed = 0.5;
		break;
	case DEFEND:
		animation = "attack";
		animSpeed = 0.5;
		break;
	}
	if (getStatusChanged() == true) {
		getAnimation()->setAnimation(animation, animSpeed);
		setStatusChanged(false);
	}
	getAnimation()->update(1);
}

void Knight::move()
{
	findDirection();
	setIsoLocation(Vector3D(getIsoLocation().x + (getMovement()*mXDirMod), getIsoLocation().y+ (getMovement()*mYDirMod), getIsoLocation().z));
}

void Knight::dying()
{

}

Knight::~Knight()
{
}
