#include "Projectile.h"
#include "Unit.h"


Projectile::Projectile(GameMath::Vector3D nLoc, std::shared_ptr<Unit> nTargetUnit, float nAttackDamage, ProjectileType nType, int nPlayer) : mTargetUnit(nTargetUnit),mLocation(nLoc), mAttackDamage(nAttackDamage),mPlayer(nPlayer)
{
	mAttackName = nType.getName();
	mAttackType = nType.getAttackType();
	mProjectileSpeed = nType.getMoveSpeed();
	mTotalFrames = nType.getTotalFrames();
	mAnim = nType.getAnimation();
	mAnim.setAnimation("move", 0.1);
}

Projectile::Projectile(GameMath::Vector3D nLoc, GameMath::Vector3D mTargetLoc, bool nDmgOrHeal, float nAttackDamage, ProjectileType nType, int nPlayer) : mTargetLoc(mTargetLoc), mLocation(nLoc), mAttackDamage(nAttackDamage), mPlayer(nPlayer), mDmgOrHeal(nDmgOrHeal)
{
	mAttackName = nType.getName();
	mAttackType = nType.getAttackType();
	mProjectileSpeed = nType.getMoveSpeed();
	mTotalFrames = nType.getTotalFrames();
	mAnim = nType.getAnimation();
	mTargetUnit = nullptr;
	mAnim.setAnimation("move", 0.1);
}

bool Projectile::Update(float seconds)
{
	if (mTargetUnit != nullptr)
	{
		mDirection = (mTargetUnit->getIsoLocation() - mLocation).normalize();
		mLocation = mLocation + (mDirection * mProjectileSpeed* seconds);
		if (mTargetUnit->collide(mLocation) == true)
		{
			mTargetUnit->hit(mAttackDamage,nullptr);
			return true;
		}
	}
	mAnim.update(seconds);
	return false;
}

void Projectile::Draw(const float& xScreenLoc, const float& yScreenLoc, const float& zScreenLoc, textureLoader& txtrLoader)
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	std::vector<GLuint> texture = txtrLoader.retrieveProjectileTexture(mAttackName);
	double x = mLocation.x - 0.5;
	double y = mLocation.y - 0.5;

	int dir = getLookDirection();
	dir -= 1;
	if (dir < 0) { dir = 7; }

	glLoadIdentity();
	glTranslatef(xScreenLoc, yScreenLoc, zScreenLoc);
	glTranslatef(x - y, (x + y) * 0.5, 0);


	if (mAttackName == "arrow")
	{
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glBegin(GL_QUADS);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f((0.0f) + (dir / 8), (1.0f / 3 )); glVertex3f(-2.0f, -1.0f, 1.0f);
		glTexCoord2f((1.0f / 8) + (dir / 8), (1.0f / 3)); glVertex3f(2.0f, -1.0f, 1.0f);
		glTexCoord2f((1.0f / 8) + (dir / 8), 0.0f); glVertex3f(2.0f, 3.0f, 1.0f);
		glTexCoord2f((0.0f) + (dir / 8), 0.0f);  glVertex3f(-2.0f, 3.0f, 1.0f);
		glEnd();
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glBegin(GL_QUADS);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f((0.0f) + (floor(mAnim.getCurrentAnimFrame()) / mTotalFrames), (1.0f) / 8 + (dir / 8.0f)); glVertex3f(-1.0f, -1.0f, 1.0f);
		glTexCoord2f((1.0f / mTotalFrames) + (floor(mAnim.getCurrentAnimFrame()) / mTotalFrames), (1.0f / 8) + (dir / 8.0f)); glVertex3f(1.0f, -1.0f, 1.0f);
		glTexCoord2f((1.0f / mTotalFrames) + (floor(mAnim.getCurrentAnimFrame()) / mTotalFrames), 0.0f + (dir / 8.0f)); glVertex3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f((0.0f / mTotalFrames) + (floor(mAnim.getCurrentAnimFrame()) / mTotalFrames), (0.0f / 8) + (dir / 8.0f));  glVertex3f(-1.0f, 1.0f, 1.0f);
		glEnd();

	}


}
int Projectile::getLookDirection()
{
	if (mDirection.x > 0.5)
	{
		if (mDirection.y > 0.5)
		{
			return NORTHEAST;
		}
		else if (mDirection.y < -0.5)
		{
			return SOUTHEAST;

		}
		else
		{
			return EAST;
		}
	}
	else if (mDirection.x < -0.5)
	{

		if (mDirection.y > 0.5)
		{
			return NORTHWEST;
		}
		else if (mDirection.y < -0.5)
		{
			return SOUTHWEST;
		}
		else
		{
			return WEST;
		}

	}
	else
	{
		if (mDirection.y > 0.5)
		{
			return NORTH;
		}
		else if (mDirection.y < -0.5)
		{
			return SOUTH;
		}
	}
}

Projectile::~Projectile()
{
}
