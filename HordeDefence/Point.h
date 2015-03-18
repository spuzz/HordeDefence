#pragma once
#include <utility>

using std::pair;
class Point
{
public:
	Point(int inxLoc, int inyLoc);
	virtual ~Point();

	const int getxLoc(void) const { return mLocation.first; }
	const int getyLoc(void) const { return mLocation.second; }

	void setxLoc(int inxLoc) { mLocation.first = inxLoc; }
	void setyLoc(int inyLoc) { mLocation.first = inyLoc; }
private:
	pair<int, int> mLocation;
};

