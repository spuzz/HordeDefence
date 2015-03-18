#include "Tile.h"

Tile::Tile() : mID(-1), mLocation(Point(-1, -1)), mWidth(-1), mHeight(-1), mWalkable(false)
{

}
Tile::Tile(int inID, int inxLoc, int inyLoc, int inWidth, int inHeight, bool inWalkable) 
	: Tile(inID,Point(inxLoc,inyLoc),inWidth,inHeight,inWalkable)
{

}
Tile::Tile(int inID, Point inLocation, int inWidth, int inHeight, bool inWalkable) 
	: mLocation(inLocation), mID(inID), mWidth(inWidth), mHeight(mHeight), mWalkable(inWalkable)
{

}


Tile::~Tile()
{
}
