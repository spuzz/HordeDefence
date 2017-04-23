#include "Tile.h"

Tile::Tile() : mID(-1), mWidth(-1), mHeight(-1), mWalkable(false), mBoundingBox(Vector3D(-1.0f, -1.0f, 0.0f),0,0)
{
	setIsoLocation(Vector3D(-1.0f, -1.0f,0.0f));
}
Tile::Tile(int inID, int inxLoc, int inyLoc, int inWidth, int inHeight, bool inWalkable) 
	: Tile(inID, Vector3D(float(inxLoc), float(inyLoc), 0),inWidth,inHeight,inWalkable)
{

}
Tile::Tile(int inID, Vector3D inLocation, int inWidth, int inHeight, bool inWalkable)
	: mID(inID), mWidth(inWidth), mHeight(mHeight), mWalkable(inWalkable), mBoundingBox(inLocation, inWidth, mHeight)
{
	setIsoLocation(inLocation);
}

void Tile::draw(const float& xScreenLoc, const float& yScreenLoc, const float& zScreenLoc, textureLoader& txtrLoader)
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	//if (txtrLoader.getCurrentTexture() != txtrLoader.retrieveMapTexture()[0])
	{
		//glBindTexture(GL_TEXTURE_2D, txtrLoader.retrieveMapTexture()[0]);
	}
	glLoadIdentity();
	glTranslatef(xScreenLoc, yScreenLoc, zScreenLoc);
	glTranslatef(((mIsoLocation.x) - (mIsoLocation.y)), ((mIsoLocation.x) + (mIsoLocation.y)) *0.5, 0);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);

	glTexCoord2f((0.0f / widthInTiles) + tileset_col / widthInTiles, (1.0f / heightInTiles) + (tileset_row / heightInTiles)); glVertex3f(-1.0f, -0.5f, 1.0f);
	glTexCoord2f((1.0f / widthInTiles) + tileset_col / widthInTiles, (1.0f / heightInTiles) + (tileset_row / heightInTiles)); glVertex3f(1.0f, -0.5f, 1.0f);
	glTexCoord2f((1.0f / widthInTiles) + tileset_col / widthInTiles, (0.0f / heightInTiles) + (tileset_row / heightInTiles)); glVertex3f(1.0f, 0.5f, 1.0f);
	glTexCoord2f((0.0f / widthInTiles) + tileset_col / widthInTiles, (0.0f / heightInTiles) + (tileset_row / heightInTiles)); glVertex3f(-1.0f, 0.5f, 1.0f);
	glEnd();
}


bool Tile::collide(const GameMath::Circle& circle)
{
	return GameMath::GameMath::circleToRect(circle, mBoundingBox);
}

bool Tile::collide(const GameMath::Rectangle& rect)
{
	return GameMath::GameMath::rectToRect(mBoundingBox, rect);
}

bool Tile::collide(const GameMath::Vector3D& point)
{
	return GameMath::GameMath::pointToRect(point, mBoundingBox);
}

Tile::~Tile()
{
}
 