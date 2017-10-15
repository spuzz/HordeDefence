#include "Teleporter.h"
#include "Dying.h"


Teleporter::Teleporter(shared_ptr<gridVector> inAstarMap, Vector3D inLocation, UnitType type, std::map<string,ProjectileType> nProjTypes, const int& nGameObjectID) : Unit(inAstarMap, inLocation, type, nProjTypes, nGameObjectID)
{
	getAnimation()->setAnimation("idle", 0.6);
	setTargetable(false);
}

void Teleporter::hit(const float& nDamage, Unit* nHitBy)
{

	// Count how many units reach this goal
	mCurrentHealth = mCurrentHealth - 1;
	if (mCurrentHealth <= 0)
	{

		setTargetable(false);
		setAction(shared_ptr<Action>(new Dying(this)));
	}

}

void Teleporter::attack(shared_ptr<Unit> enemy)
{
	enemy->setDead(true);
	enemy->setTargetable(false);
	hit(1, this);

}

void Teleporter::draw(const float& xScreenLoc, const float& yScreenLoc, const float& zScreenLoc, textureLoader& txtrLoader)
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	calcAABBWorldSpace();
	std::vector<GLuint> character = GetTextures(txtrLoader);

	for (auto txtr : character) {
		mIsoDepth;
		glLoadIdentity();
		glTranslatef(xScreenLoc, yScreenLoc, zScreenLoc);
		double x = getIsoLocation().x - 0.5;
		double y = getIsoLocation().y - 0.5;
		glTranslatef(x - y, (x + y) * 0.5, 0);
		if (mSelected)
		{
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
		}
		glBindTexture(GL_TEXTURE_2D, txtr);
		glBegin(GL_QUADS);
		glNormal3f(0.0f, 0.0f, 1.0f);
		int row = int(getAnimation()->getCurrentAnimFrame()) / 10;
		glTexCoord2f((0.0f) + (int(getAnimation()->getCurrentAnimFrame()) % 10), (1.0f) / 10 + (row / 10.0f)); glVertex3f(-2.0f, -1.0f, 1.0f);
		glTexCoord2f((1.0f / 10) + (int(getAnimation()->getCurrentAnimFrame()) % 10), (1.0f / 10) + (row / 10.0f)); glVertex3f(2.0f, -1.0f, 1.0f);
		glTexCoord2f((1.0f / 10) + (int(getAnimation()->getCurrentAnimFrame()) % 10), 0.0f + (row / 10.0f)); glVertex3f(2.0f, 3.0f, 1.0f);
		glTexCoord2f((0.0f / 10) + (int(getAnimation()->getCurrentAnimFrame()) % 10), (0.0f) + (row / 10.0f));  glVertex3f(-2.0f, 3.0f, 1.0f);
		glEnd();


	}
}

std::vector<GLuint> Teleporter::GetTextures(textureLoader& txtrLoader)
{
	return txtrLoader.retrieveUnitTexture("Teleporter", 0);
}

Teleporter::~Teleporter()
{
}
