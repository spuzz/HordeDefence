#include "Tile.h"

Tile::Tile() : mID(-1), mWidth(-1), mHeight(-1), mWalkable(false)
{
	setIsoLocation(Vect(-1.0f, -1.0f,0.0f));
}
Tile::Tile(int inID, int inxLoc, int inyLoc, int inWidth, int inHeight, bool inWalkable) 
	: Tile(inID, Vect(float(inxLoc), float(inyLoc), 0),inWidth,inHeight,inWalkable)
{

}
Tile::Tile(int inID,Vect inLocation, int inWidth, int inHeight, bool inWalkable)
	: mID(inID), mWidth(inWidth), mHeight(mHeight), mWalkable(inWalkable)
{
	setIsoLocation(inLocation);
}

void Tile::draw(const float& xScreenLoc, const float& yScreenLoc, const float& zScreenLoc, textureLoader& txtrLoader)
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	if (txtrLoader.getCurrentTexture() != texture[0]) 
	{
		glBindTexture(GL_TEXTURE_2D, texture[0]);
	}
	glLoadIdentity();
	glTranslatef(xScreenLoc, yScreenLoc, zScreenLoc);
	glTranslatef(((mIsoLocation.getX() - 1) - (mIsoLocation.getY() - 1)), ((mIsoLocation.getX() - 1) + (mIsoLocation.getY() - 1)) *0.5, 0);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);

	glTexCoord2f((0.0f / widthInTiles) + tileset_col / widthInTiles, (1.0f / heightInTiles) + (tileset_row / heightInTiles)); glVertex3f(-1.0f, -0.5f, 1.0f);
	glTexCoord2f((1.0f / widthInTiles) + tileset_col / widthInTiles, (1.0f / heightInTiles) + (tileset_row / heightInTiles)); glVertex3f(1.0f, -0.5f, 1.0f);
	glTexCoord2f((1.0f / widthInTiles) + tileset_col / widthInTiles, (0.0f / heightInTiles) + (tileset_row / heightInTiles)); glVertex3f(1.0f, 0.5f, 1.0f);
	glTexCoord2f((0.0f / widthInTiles) + tileset_col / widthInTiles, (0.0f / heightInTiles) + (tileset_row / heightInTiles)); glVertex3f(-1.0f, 0.5f, 1.0f);
	glEnd();
}

Tile::~Tile()
{
}
 