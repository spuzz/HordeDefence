#pragma once

#include "Point.h"
class Tile
{
public:
	Tile();
	Tile(int inID,int inxLoc,int inyLoc, int inWidth, int inHeight,bool inWalkable);
	Tile(int inID, Point location, int inWidth, int inHeight,bool inWalkable);
	virtual ~Tile();

	int const getID(void) const { return mID; }
	Point const getLocation(void) const { return mLocation; }
	int const getWidth(void) { return mWidth; }
	int const getHeight(void) { return mHeight;  }

	void setLocation(Point inLocation) { mLocation = inLocation;  }
	void setXLocation(int inX) { mLocation.setxLoc(inX); }
	void setYLocation(int inY) { mLocation.setyLoc(inY); }
	void setWidth(int inWidth) { mWidth = inWidth;  }
	void setheight(int inHeight) { mWidth = inHeight; }
protected:

private:
	int mID, mWidth, mHeight;
	Point mLocation;
	bool mWalkable;

};

