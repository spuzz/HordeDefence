#include "Unit.h"
#include <algorithm>
#include "GridAstar.h"
#include "tinyxml2.h"
#include "Attack.h"
#include "Dying.h"
#include "Collision.h"
#include "AttackMove.h"
#include "MoveTo.h"
Unit::Unit(shared_ptr<gridVector> inAstarMap, Vector3D inLocation, UnitType type,const int& nGameObjectID) : mAnimation(new Animation()), mAstarGrid(inAstarMap),mBoundingBox(inLocation,type.getSize())
{
	
	mGameObjectID = nGameObjectID;
	inLocation.x== -1 ? setIsoLocation(Vector3D(0.0f, 0.0f, 0.0f)) : setIsoLocation(Vector3D(inLocation.x, inLocation.y, 0.0f));
	setZHeight(inLocation.z);
	mBoundingBox.mLocation.z = 0;
	mBoundingBox.mLocation.x = mIsoLocation.x;
	mBoundingBox.mLocation.y = mIsoLocation.y;
	calcScreenLocation();
	//setLocation(Vector3D(getIsoLocation().x - getIsoLocation().y, (getIsoLocation().x + getIsoLocation().y) * 0.5, 0));
	setAABBModelSpace(Vector3D(-2, -2, 0), Vector3D(2, 2, 1));
	calcAABBWorldSpace();
	//mIsoLocation.setZ(3);

	setMovement(type.getMoveSpeed());
	setMaxHealth(type.getHitPoints());
	setCurrentHealth(type.getHitPoints());
	setRange(type.getRange());


	setTarget(Vector3D(inLocation.x,inLocation.y,0));
	mTargetUnit = nullptr;
	setGender(0);
	setAttackSpeed(type.getAttackSpeed());
	setSize(type.getSize());
	setAnimation(type.getAnimation());
	setRace(type.getName());
	setClassType("Knight");
	setStatus(IDLE);
	getAnimation()->setAnimation("idle", 0.3);

	mSelected = false;
	setTargetable(true);
	mCanUseEquipment = false;
	mAttackMove = false;
}
void Unit::attack()
{
	getAnimation()->setAnimation("attack", 0.5);
}

void Unit::update(float nSeconds)
{
	if (mDead)
	{
		return;
	}
	if (mTargetUnit != nullptr && (mTargetUnit->getMapLocation().x != mTarget.x ||  mTargetUnit->getMapLocation().y != mTarget.y))
	{
		setTarget(mTargetUnit->getIsoLocation());
		findPath();
	}
	if (mAction == nullptr || mAction->update(nSeconds))
	{
		mAction = std::shared_ptr<Action>(new Idle(this, nullptr));
	}

	getAnimation()->update(nSeconds);
	//string animation;
	//float animSpeed;
	//if (getPath().size() != 0) {
	//	setStatus(MOVE);
	//}
	//else {
	//	if (!GameMath::GameMath::pointToCircle(mTarget, mBoundingBox))
	//	{
	//		setStatus(MOVE);
	//	}
	//	else
	//	{
	//		setStatus(IDLE);
	//	}
	//	
	//}
	//switch (getStatus()) {
	//case IDLE:
	//	animation = "idle";
	//	animSpeed = 0.4;
	//	break;
	//case MOVE:
	//	animation = "move";
	//	animSpeed = 0.5;
	//	move();
	//	break;
	//case ATTACK:
	//	animation = "attack";
	//	animSpeed = 0.5;
	//	break;
	//case ATTACKMOVE:
	//	animation = "move";
	//	animSpeed = 0.5;
	//	break;
	//case DEFEND:
	//	animation = "attack";
	//	animSpeed = 0.5;
	//	break;
	//}
	//if (getStatusChanged() == true) {
	//	getAnimation()->setAnimation(animation, animSpeed);
	//	setStatusChanged(false);
	//}
	
}

void Unit::move(Vector3D nMoveVec)
{
	if (mCollisionSystem->CheckUnitCollision(getObjectID(), GameMath::Circle(nMoveVec, getBoundingBox().mRadius), true) == 0)
	{
		setIsoLocation(nMoveVec);
		mBoundingBox.mLocation.x = mIsoLocation.x;
		mBoundingBox.mLocation.y = mIsoLocation.y;
		calcScreenLocation();
	}

	//setLocation(Vector3D(getIsoLocation().x - getIsoLocation().y, (getIsoLocation().x + getIsoLocation().y) * 0.5, 0));
}

void Unit::hit(const float& nDamage, Unit* nHitBy)
{
	mCurrentHealth -= nDamage;
	if (mCurrentHealth <= 0)
	{

		setTargetable(false);
		setAction(shared_ptr<Action>(new Dying(this)));
	}
}
void Unit::damage(const float& nDamage)
{
	mCurrentHealth -= nDamage;
	if (mCurrentHealth <= 0)
	{

		setTargetable(false);
		setAction(shared_ptr<Action>(new Dying(this)));
	}
}

void Unit::heal(const float& nHeal)
{
	mCurrentHealth += nHeal;
	if (mCurrentHealth > mMaxHealth)
	{
		mCurrentHealth = mMaxHealth;
	}
}



void Unit::dying()
{

}

void Unit::setStatus(statuses inStatus) 
{ 
	if (mStatus != inStatus) {
		setStatusChanged(true);
		mStatus = inStatus;
	}

}


void Unit::changeAnim(string anim)
{
	
}
void Unit::newTarget(GameMath::Vector3D inTarget)
{
	mTargetUnit = nullptr;
	setTarget(inTarget);
	mAttackMove = false;
	findPath();
}
void Unit::newTarget(shared_ptr<Unit> nUnit)
{
	mTargetUnit = nUnit;
	setTarget(mTargetUnit->getIsoLocation());
	findPath();
}

void Unit::attackMove(GameMath::Vector3D inTarget)
{
	mTargetUnit = nullptr;
	setTarget(inTarget);
	mAttackMove = true;
	findPath();
}

void Unit::findPath()
{
	shared_ptr<AstarLifeCycleHandler> handler(new AstarHandler(getChangeablePath()));
	astar = shared_ptr<AstarService>(new GridAstarService(*getAstarGrid(), 10, -1));
	astar->submitPath(getIsoLocation().x, getIsoLocation().y, mTarget.x, mTarget.y, handler);
}

Vector3D Unit::findDirection()
{
	// Use top left of tile/unit location unless it is the final target
	Vector3D target;

	if (mPath.size() != 0)
	{
		target = Vector3D(mPath[mPath.size() - 1].first, mPath[mPath.size() - 1].second, 0);
	}
	else
	{
		target = mTarget;
	}
	
	if (GameMath::GameMath::pointToCircle(target, mBoundingBox))
	{

		if (mPath.size() >= 1)
		{
			mPath.erase(--mPath.end());
			if (mPath.size() > 0)
			{
				target = Vector3D(mPath[mPath.size() - 1].first, mPath[mPath.size() - 1].second, 0);
			}
			else
			{
				target = mTarget;
			}
			
		}
		
	}


	Vector3D direction = target - getIsoLocation();
	direction.z = 0;
	mDirection = direction.normalize();
	setLookDirection();
	return mDirection;
}

shared_ptr<Unit> Unit::getNearestEnemyUnit(const int& nRange)
{

	auto units = mCollisionSystem->getUnitsInRange(mGameObjectID, GameMath::Circle(getIsoLocation(), nRange), mPlayer, 2);

	// If no units found in range return nullptr
	if (units.size() == 0)
	{
		return nullptr;
	}
	int mDistance = -1;
	shared_ptr<Unit> mTargetUnit;

	// Check through all units and determine which distance is the smallest using magnitude of vector between 2 units
	for (auto unit : units)
	{
		if (mDistance == -1 || (unit->getBoundingBox().mLocation - getIsoLocation()).GetSquaredMagnitude() < mDistance)
		{
			mTargetUnit = unit;
		}
	}

	return mTargetUnit;
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

void Unit::draw(const float& xScreenLoc, const float& yScreenLoc, const float& zScreenLoc, textureLoader& txtrLoader)
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	calcAABBWorldSpace();
	std::vector<GLuint> character = GetTextures(txtrLoader);
	double x = getIsoLocation().x - 0.5;
	double y = getIsoLocation().y - 0.5;
	
	int dir = getDirection();
	dir -= 1;
	if (dir < 0) { dir = 7; }
	if (mSelected)
	{
		glLoadIdentity();
		glTranslatef(xScreenLoc, yScreenLoc, zScreenLoc);
		glTranslatef(mScreenLocation.x, mScreenLocation.y, 0);
		int test = 0;
		glDisable(GL_TEXTURE_2D);
		glBegin(GL_LINE_LOOP);
		for (int ii = 0; ii < 300; ii++)
		{  
			float theta = 2.0f * 3.1415926f * float(ii) / float(300);//get the current angle 

			float x = mBoundingBox.mRadius * cosf(theta);//calculate the x component 
			float y = mBoundingBox.mRadius  * sinf(theta);//calculate the y component 


			glVertex3f(mScreenLocation.x, mScreenLocation.y, 1.0f);//output vertex 

		}
		glEnd();
		
		glEnable(GL_TEXTURE_2D);
		
	}

	if (mSelected || mPlayer != 0)
	{
		drawUnitHealth(txtrLoader, x, y, xScreenLoc, yScreenLoc, zScreenLoc);
	}
	for (auto txtr : character) {
		glLoadIdentity();
		glTranslatef(xScreenLoc, yScreenLoc, zScreenLoc);
		glTranslatef(x - y, (x + y) * 0.5, 0);


		glBindTexture(GL_TEXTURE_2D, txtr);
		glBegin(GL_QUADS);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f((0.0f) + (floor(getAnimation()->getCurrentAnimFrame()) / 32), (1.0f) / 8 + (dir / 8.0f)); glVertex3f(-2.0f, -1.0f, 1.0f);
		glTexCoord2f((1.0f / 32) + (floor(getAnimation()->getCurrentAnimFrame()) / 32), (1.0f / 8) + (dir / 8.0f)); glVertex3f(2.0f, -1.0f, 1.0f);
		glTexCoord2f((1.0f / 32) + (floor(getAnimation()->getCurrentAnimFrame()) / 32), 0.0f + (dir / 8.0f)); glVertex3f(2.0f, 3.0f, 1.0f);
		glTexCoord2f((0.0f / 32) + (floor(getAnimation()->getCurrentAnimFrame()) / 32), (0.0f / 8) + (dir / 8.0f));  glVertex3f(-2.0f, 3.0f, 1.0f);
		glEnd();
		

	}
}

void Unit::drawUnitHealth(textureLoader& txtrLoader,double x, double y, const float& xScreenLoc, const float& yScreenLoc, const float& zScreenLoc)
{
	glLoadIdentity();
	glTranslatef(xScreenLoc, yScreenLoc, zScreenLoc);
	glTranslatef(x - y, (x + y) * 0.5, 0);

	glBindTexture(GL_TEXTURE_2D, txtrLoader.retrieveUnitTexture("HealthBarFrame", getGender())[0]);

	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f,1.0f); glVertex3f(-1.2f, 1.5f, 1.0f);
	glTexCoord2f(1.0f ,1.0f); glVertex3f(1.2f, 1.5f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.2f, 2.3, 1.0f);
	glTexCoord2f(0.0f, 0.0f);  glVertex3f(-1.2f, 2.3, 1.0f);
	glEnd();

	float healthPerc = mCurrentHealth / mMaxHealth;
	if (healthPerc < 0)
	{
		healthPerc = 0;
	}
	if (mPlayer == 0)
	{
		glBindTexture(GL_TEXTURE_2D, txtrLoader.retrieveUnitTexture("GreenHealthBar", getGender())[0]);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, txtrLoader.retrieveUnitTexture("RedHealthBar", getGender())[0]);
	}

	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.5f, 1.0f);
	glTexCoord2f(1.0f * healthPerc, 1.0f); glVertex3f(-1.0f + (2 * healthPerc), 1.5f, 1.0f);
	glTexCoord2f(1.0f * healthPerc, 0.0f); glVertex3f(-1.0f + (2 * healthPerc), 2.3f, 1.0f);
	glTexCoord2f(0.0f, 0.0f);  glVertex3f(-1.0f, 2.3f, 1.0f);
	glEnd();
}

std::vector<GLuint> Unit::GetTextures(textureLoader& txtrLoader)
{
	return txtrLoader.retrieveUnitTexture(getRace(), getGender());
}

bool Unit::collide(const GameMath::Circle& circle)
{
	return GameMath::GameMath::circleToCircle(mBoundingBox, circle);
}

bool Unit::collide(const GameMath::Rectangle& rect)
{
	return GameMath::GameMath::circleToRect(mBoundingBox, rect);
}

bool Unit::collide(const GameMath::Vector3D& point)
{
	return GameMath::GameMath::pointToCircle(point,mBoundingBox);
}

void Unit::setLookDirection()
{
	if (mDirection.x > 0.5)
	{
		if (mDirection.y > 0.5)
		{
			mFacing = NORTHEAST;
		}
		else if (mDirection.y < -0.5)
		{
			mFacing = SOUTHEAST;
			
		}
		else
		{
			mFacing = EAST;
		}
	}
	else if (mDirection.x < -0.5)
	{

		if (mDirection.y > 0.5)
		{
			mFacing = NORTHWEST;
		}
		else if (mDirection.y < -0.5)
		{
			mFacing = SOUTHWEST;
		}
		else
		{
			mFacing = WEST;
		}

	}
	else
	{
		if (mDirection.y > 0.5)
		{
			mFacing = NORTH;
		}
		else if (mDirection.y < -0.5)
		{
			mFacing = SOUTH;
		}
	}
}

void Unit::equipWeapon(const Weapon &inWeapon)
{
		mEquippedWeapon = inWeapon;
}

void Unit::equipArmor(const Armor &inArmor)
{
	mEquippedArmor = inArmor;
}

void Unit::equipOffhand(const Offhand &inOffhand)
{
	mEquippedOffhand = inOffhand;
}

void Unit::SetEquipment(const string& nWeapon, const string& nArmor, const string& nOffhand)
{
	Weapon wep(nWeapon);
	Armor armor(nArmor);
	Offhand offhand(nOffhand);
	equipWeapon(wep);
	equipArmor(armor);
	equipOffhand(offhand);
}

Unit::~Unit()
{

}
