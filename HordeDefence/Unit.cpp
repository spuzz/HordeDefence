#include "Unit.h"
#include <algorithm>
#include "GridAstar.h"
#include "tinyxml2.h"
#include "Attack.h"
#include "Dying.h"
#include "Collision.h"
#include "AttackMove.h"
#include "MoveTo.h"
#include "Rectangle.h"
Unit::Unit(shared_ptr<gridVector> inAstarMap, Vector3D inLocation, UnitType type, std::map<string,ProjectileType> nProjTypes ,const int& nGameObjectID) : mAnimation(new Animation()), mAstarGrid(inAstarMap),mBoundingBox(inLocation,type.getSize())
{
	mProjectilesTypes = nProjTypes;
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
	setAttackType(type.getAttackType());
	setAttackDamage(type.getAttackDamage());
	setArmor(type.getArmor());

	//setTarget(Vector3D(inLocation.x,inLocation.y,0));
	mTargetUnit = nullptr;
	setGender(0);
	setAttackSpeed(type.getAttackSpeed());
	setSize(type.getSize());
	setAnimation(type.getAnimation());
	setRace(type.getName());
	setClassType(type.getClass());
	setTotalFrames(type.getTotalFrames());
	setStatus(IDLE);
	getAnimation()->setAnimation("idle", 0.3);

	mSelected = false;
	setTargetable(true);
	mCanUseEquipment = false;
	mAttackMove = false;
	mActionCD = 0;
}


void Unit::update(float nSeconds)
{
	if (mDead)
	{
		return;
	}
	mActionCD -= nSeconds;
	if (mActionCD <= 0)
	{
		mActionCD = 0;
	}
	//if (mTargetUnit != nullptr && (mTargetUnit->getMapLocation().x != mTarget.x ||  mTargetUnit->getMapLocation().y != mTarget.y))
	//{
	//	setTarget(mTargetUnit->getIsoLocation());
	//	findPath();
	//}
	if (mAction == nullptr || mAction->update(nSeconds))
	{
		mAction = std::shared_ptr<Action>(new Idle(this));
	}

	getAnimation()->update(nSeconds);
	
}

void Unit::attack(shared_ptr<Unit> enemy)
{
	if (mActionCD == 0)
	{
		getAnimation()->setAnimation("attack", 0.3);
		if (mAttackType == "range" || mAttackType == "magic")
		{
			if (mAttackType == "magic")
			{
				mProjectiles.push_back(Projectile(getIsoLocation(), enemy, mAttackDamage, mProjectilesTypes["Fireball"], mPlayer));
			}
			else
			{
				mProjectiles.push_back(Projectile(getIsoLocation(), enemy, mAttackDamage, mProjectilesTypes["Arrow"], mPlayer));
			}
			
		}
		else
		{
			enemy->hit(mAttackDamage, this);
		}
		mActionCD += mAttackSpeedPerSecond;
	}
	
}

void Unit::move(Vector3D nMoveVec)
{
	if (mCollisionSystem->CheckUnitCollision(getObjectID(), GameMath::Circle(nMoveVec, getBoundingBox().mRadius), true) == 0 && mCollisionSystem->CheckTileCollision(GameMath::Circle(nMoveVec, getBoundingBox().mRadius), true) == true)
	{
		setIsoLocation(nMoveVec);
		mBoundingBox.mLocation.x = mIsoLocation.x;
		mBoundingBox.mLocation.y = mIsoLocation.y;
		calcScreenLocation();
	}
	//else 
	//{
	//	float yMove = nMoveVec.y;
	//	nMoveVec.y = getIsoLocation().y;
	//	if (mCollisionSystem->CheckUnitCollision(getObjectID(), GameMath::Circle(nMoveVec, getBoundingBox().mRadius), true) == 0 && mCollisionSystem->CheckTileCollision(GameMath::Circle(nMoveVec, getBoundingBox().mRadius), true) == true)
	//	{
	//		
	//		setIsoLocation(nMoveVec);
	//		mBoundingBox.mLocation.x = mIsoLocation.x;
	//		mBoundingBox.mLocation.y = mIsoLocation.y;
	//		calcScreenLocation();
	//	}
	//	else
	//	{
	//		nMoveVec.y =  yMove;
	//		nMoveVec.x = getIsoLocation().x;
	//		if (mCollisionSystem->CheckUnitCollision(getObjectID(), GameMath::Circle(nMoveVec, getBoundingBox().mRadius), true) == 0 && mCollisionSystem->CheckTileCollision(GameMath::Circle(nMoveVec, getBoundingBox().mRadius), true) == true)
	//		{
	//			setIsoLocation(nMoveVec);
	//			mBoundingBox.mLocation.x = mIsoLocation.x;
	//			mBoundingBox.mLocation.y = mIsoLocation.y;
	//			calcScreenLocation();
	//		}
	//	}
	//}


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

void Unit::setTarget(GameMath::Vector3D inTarget) 
{ 
	if (mTarget.x != inTarget.x && mTarget.y != inTarget.y)
	{
		mTarget = inTarget;
		findPath();
	}
}
void Unit::setTargetUnit(shared_ptr<Unit> nUnit) 
{ 
	mTargetUnit = nUnit;
	if (mTarget.x != nUnit->getIsoLocation().x && mTarget.y != nUnit->getIsoLocation().y)
	{
		mTarget = nUnit->getIsoLocation();
		findPath();
	}

}

void Unit::newTarget(GameMath::Vector3D inTarget)
{
	setAction(shared_ptr<Action>(new MoveTo(this, getTarget())));
}
void Unit::newTarget(shared_ptr<Unit> nUnit)
{
	setAction(shared_ptr<Action>(new Attack(this, mTargetUnit)));
}

void Unit::attackMove(GameMath::Vector3D inTarget)
{
	setAction(shared_ptr<Action>(new AttackMove(this, inTarget)));
}

void Unit::findPath()
{
	shared_ptr<AstarLifeCycleHandler> handler(new AstarHandler(this));
	astar = shared_ptr<AstarService>(new GridAstarService(*getAstarGrid(), 10, -1));
	astar->submitPath(getIsoLocation().x, getIsoLocation().y, mTarget.x, mTarget.y, handler);
}

locationFloatVector Unit::ConvertAndAddMidPoints(locationVector nPath)
{
	locationFloatVector path;
	for (auto node : nPath)
	{
		path.push_back(std::pair<float, float>(node.first + 0.5, node.second + 0.5));
	}
	return path;
}

Vector3D Unit::findDirection()
{
	// Use top left of tile/unit location unless it is the final target
	Vector3D target(-1,-1,-1);

 	if (mPath.size() != 0)
	{
		target = Vector3D(mPath[0].first, mPath[0].second, 0);
	}

	if (mPath.size() > 1)
	{
		std::vector<Vector3D> tiles = mCollisionSystem->TilesUnderLine(getIsoLocation().x, getIsoLocation().y, mPath[1].first, mPath[1].second);
		bool canGetToNextNode = true;
		for (int a = 0; a < tiles.size(); a++)
		{
			if (mCollisionSystem->CheckTileCollision(tiles[a], 0) == false)
			{
				canGetToNextNode = false;
				break;
			}
		}
		if (canGetToNextNode == true)
		{
			mPath.erase(mPath.begin());
			target = Vector3D(mPath[0].first, mPath[0].second, 0);
		}
	}
	if (GameMath::GameMath::pointToCircle(target, mBoundingBox))
	{

		if (mPath.size() >= 1)
		{
			mPath.erase(mPath.begin());
			if (mPath.size() > 0)
			{
				target = Vector3D(mPath[0].first, mPath[0].second, 0);
			}
			
		}
		
	}


	if (target.x != -1)
	{
		Vector3D direction = target - getIsoLocation();
		Vector3D avoidForce(0, 0, 0);
		direction.z = 0;
		direction = direction.normalize();
		float scale = 0;
		std::vector<shared_ptr<Unit>> units = mCollisionSystem->getUnitsInRange(this->getObjectID(), GameMath::Circle(target, 12), 0, 0);

		GameMath::Vector3D closest(-1,-1,-1);
		Vector3D result(-1, -1, -1);
		float distance = Vector3D(abs(getIsoLocation().x - target.x), abs(getIsoLocation().y - target.y), 0).GetSquaredMagnitude();//(target - getIsoLocation()).GetSquaredMagnitude();
		bool foundCollision = false;
		Vector3D rightLineStart, rightLineEnd, leftLineStart, leftLineEnd;
		Vector3D normal(direction.y, -direction.x, 0);
		Vector3D oppNormal(-direction.y, direction.x, 0);
		normal = normal.normalize();
		oppNormal = oppNormal.normalize();
		Vector3D normalTimesRadius(0, 0, 0);
		Vector3D oppNormalTimesRadius(0, 0, 0);
		normalTimesRadius = normal * (mBoundingBox.mRadius);
		oppNormalTimesRadius = oppNormal * (mBoundingBox.mRadius);

		rightLineStart = Vector3D(getIsoLocation().x + normalTimesRadius.x, getIsoLocation().y + normalTimesRadius.y, 0);
		rightLineEnd = rightLineStart + direction * 3;
		leftLineStart = Vector3D(getIsoLocation().x + oppNormalTimesRadius.x, getIsoLocation().y + oppNormalTimesRadius.y, 0);
		leftLineEnd = leftLineStart + direction * 3;
		for (auto unit : units)
		{
			if (mTargetUnit == nullptr || (mTargetUnit->getObjectID() != unit->getObjectID()))
			{
				
				if (GameMath::GameMath::lineToCircle(leftLineStart, leftLineEnd, unit->getBoundingBox(),result))
				{
					foundCollision = true;
					float curDistance = Vector3D(abs(leftLineStart.x - result.x), abs(leftLineStart.y - result.y), 0).GetSquaredMagnitude();
					if (curDistance < distance)
					{
						foundCollision = true;
						closest = result;
						avoidForce = normal * 1;
						scale = mBoundingBox.mRadius / sqrt(curDistance);
						distance = curDistance;
					}

					//float curDistance = Vector3D(abs(getIsoLocation().x - unit->getIsoLocation().x), abs(getIsoLocation().y - unit->getIsoLocation().y), 0).GetSquaredMagnitude();
					//if (curDistance < distance)
					//{
					//	//closest = Vector3D(unit->getBoundingBox().mLocation.x, unit->getBoundingBox().mLocation.y, 0);
					//	closest = GameMath::GameMath::lineToCirclePoint(leftLineStart, leftLineEnd, unit->getBoundingBox());
					//	distance = curDistance;

					//}
				}
				if (GameMath::GameMath::lineToCircle(rightLineStart, rightLineEnd, unit->getBoundingBox(),result))
				{
					float curDistance = Vector3D(abs(rightLineStart.x - result.x), abs(rightLineStart.y - result.y), 0).GetSquaredMagnitude();
					if (curDistance < distance)
					{
						foundCollision = true;
						closest = result;
						avoidForce = oppNormal * 1;
						scale = mBoundingBox.mRadius / sqrt(curDistance);
 						distance = curDistance;
					}
					
					//float curDistance = Vector3D(abs(getIsoLocation().x - unit->getIsoLocation().x), abs(getIsoLocation().y - unit->getIsoLocation().y), 0).GetSquaredMagnitude();
					//if (curDistance < distance)
					//{
					//	//closest = Vector3D(unit->getBoundingBox().mLocation.x, unit->getBoundingBox().mLocation.y, 0);
					//	closest = GameMath::GameMath::lineToCirclePoint(rightLineStart, rightLineEnd, unit->getBoundingBox());
					//	distance = curDistance;
					//}
				}
			}

		}
		std::vector<Vector3D> tiles = mCollisionSystem->TilesUnderLine(leftLineStart.x, leftLineStart.y, leftLineEnd.x, leftLineEnd.y);
		for (auto tile : tiles)
		{
			
			if (mCollisionSystem->CheckTileCollision(tile, 0) == false)
			{
				foundCollision = true;
				result = GameMath::GameMath::LineToRectPoint(leftLineStart, leftLineEnd, GameMath::Rectangle(tile, 1, 1));
				float curDistance = Vector3D(abs(leftLineStart.x - result.x), abs(leftLineStart.y - result.y), 0).GetSquaredMagnitude();
				if (curDistance < distance)
				{
					closest = result;
					avoidForce = normal * 1;
					distance = curDistance;
					scale = 0.5*0.5 / sqrt(curDistance);
				}

			}
		}

		tiles = mCollisionSystem->TilesUnderLine(rightLineStart.x, rightLineStart.y, rightLineEnd.x, rightLineEnd.y);
		for (auto tile : tiles)
		{
			if (mCollisionSystem->CheckTileCollision(tile, 0) == false)
			{
				foundCollision = true;
				result = GameMath::GameMath::LineToRectPoint(rightLineStart, rightLineEnd, GameMath::Rectangle(tile, 1, 1));
				float curDistance = Vector3D(abs(rightLineStart.x - result.x), abs(rightLineStart.y - result.y), 0).GetSquaredMagnitude();
				if (curDistance < distance)
				{
					closest = result;
					avoidForce = oppNormal * 1;
					distance = curDistance;
					scale = 0.5*0.5 / sqrt(curDistance);
				}
			}
		}

		//if (foundCollision && closest.x != -1)
		//{
		//	
		//	avoidForce.x = getIsoLocation().x- closestCollsionPoint.x;
		//	avoidForce.y = getIsoLocation().y - closestCollsionPoint.y;
		//	//if (avoidForce.x - avoidForce.y < 0.02)
		//	//{
		//	//	avoidForce = avoidForce + normal;
		//	//}
		//	avoidForce = avoidForce.normalize ();
		//	avoidForce = avoidForce * 1.0;
		//}
		//

		mDirection = (direction *  (1 - scale) + (avoidForce * scale));
		mDirection = mDirection.normalize();
		setLookDirection();
	}
	else
	{
		mDirection = Vector3D(0, 0, 0);
	}

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
	float mDistance = -1;
	shared_ptr<Unit> mTargetUnit;

	// Check through all units and determine which distance is the smallest using magnitude of vector between 2 units
	for (auto unit : units)
	{
		float currentDistance = (unit->getBoundingBox().mLocation - getIsoLocation()).GetSquaredMagnitude();
		if (mDistance == -1 || currentDistance < mDistance)
		{
			mDistance = currentDistance;
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
	//if (mSelected)
	//{
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


			glVertex3f(x - y, (x + y) * 0.5, 1.0f);//output vertex 

		}
		glEnd();
		
		glEnable(GL_TEXTURE_2D);
	//	
	//}

	if (mSelected || mPlayer != 0)
	{
		//drawUnitHealth(txtrLoader, x, y, xScreenLoc, yScreenLoc, zScreenLoc);
	}
	for (auto txtr : character) {
		glLoadIdentity();
		glTranslatef(xScreenLoc, yScreenLoc, zScreenLoc);
		glTranslatef(x - y, (x + y) * 0.5, 0);


		glBindTexture(GL_TEXTURE_2D, txtr);
		glBegin(GL_QUADS);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f((0.0f) + (floor(getAnimation()->getCurrentAnimFrame()) / mTotalFrames), (1.0f) / 8 + (dir / 8.0f)); glVertex3f(-2.0f, -1.0f, 1.0f);
		glTexCoord2f((1.0f / mTotalFrames) + (floor(getAnimation()->getCurrentAnimFrame()) / mTotalFrames), (1.0f / 8) + (dir / 8.0f)); glVertex3f(2.0f, -1.0f, 1.0f);
		glTexCoord2f((1.0f / mTotalFrames) + (floor(getAnimation()->getCurrentAnimFrame()) / mTotalFrames), 0.0f + (dir / 8.0f)); glVertex3f(2.0f, 3.0f, 1.0f);
		glTexCoord2f((0.0f / mTotalFrames) + (floor(getAnimation()->getCurrentAnimFrame()) / mTotalFrames), (0.0f / 8) + (dir / 8.0f));  glVertex3f(-2.0f, 3.0f, 1.0f);
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

locationFloatVector Unit::smoothPath(locationFloatVector inPath)
{

	if (inPath.size() == 0)
	{
		return inPath;
	}

	if (inPath[0] != std::pair<float, float>(mTarget.x, mTarget.y))
	{
		inPath.insert(inPath.begin(), std::pair<float, float>(mTarget.x, mTarget.y));
	}

	locationFloatVector outPath;
	int currentNode = inPath.size() - 1;
	for (int nextNode = inPath.size() - 2; nextNode >= 1; nextNode--)
	{
		std::vector<Vector3D> tiles = mCollisionSystem->TilesUnderLine(inPath[currentNode].first, inPath[currentNode].second, inPath[nextNode].first, inPath[nextNode].second);
		for (int a = 0; a < tiles.size(); a++)
		{
			if (mCollisionSystem->CheckTileCollision(tiles[a], 0) == false)
			{
				outPath.push_back(inPath[nextNode]);
				currentNode = nextNode;
				break;
			}
		}
	}
	// always add final node
	if (inPath.size() > 1)
	{
		outPath.push_back(inPath[0]);
	}
	return outPath;
}

Unit::~Unit()
{

}
