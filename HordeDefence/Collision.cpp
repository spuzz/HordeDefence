#include "Collision.h"



Collision::Collision(shared_ptr<gridVector> nTiles) : mTiles(nTiles)
{
}

bool Collision::addUnit(shared_ptr<Unit> nUnit)
{
	if (CheckTileCollision(nUnit->getBoundingBox(), true) == false ||  CheckUnitCollision(nUnit->getObjectID(), nUnit->getBoundingBox(), true) != 0 )
	{
		return false;
	}
	mUnits.insert(pair<int, shared_ptr<Unit>>(nUnit->getObjectID(), nUnit));
	return true;
	
}

void Collision::deleteUnit(const int& nUnitID)
{
	cleanID(nUnitID);
	mUnits.erase(nUnitID);

}

void Collision::tryMove()
{

}

bool Collision::CheckTileCollision(const Vector3D loc, bool nMoveUnit)
{
	std::pair<int, int> tileLoc(loc.x, loc.y);
	if (tileLoc.first < 0 || tileLoc.first >= mTiles->size())
	{
		return false;
	}
	if (tileLoc.second < 0 || tileLoc.second >= mTiles->at(tileLoc.first).size())
	{
		return false;
	}
	auto findTile = mTiles->at(tileLoc.first)[tileLoc.second];
	if (findTile == -1)
	{
		return false;
	}
	return true;
}

bool Collision::CheckTileCollision(const GameMath::Circle& nBoundary, bool nMoveUnit)
{
	int x, minX, maxX, y, minY, maxY;
	minX = (nBoundary.mLocation.x - nBoundary.mRadius);
	maxX = (nBoundary.mLocation.x + nBoundary.mRadius);
	minY = (nBoundary.mLocation.y - nBoundary.mRadius);
	maxY = (nBoundary.mLocation.y + nBoundary.mRadius);
	for (int a = minX; a <= maxX; a++)
	{
		for (int b = minY; b <= maxY; b++)
		{
			if (GameMath::GameMath::circleToRect(nBoundary, GameMath::Rectangle(Vector3D(a, b, 0), 1, 1)))
			{
				if (!CheckTileCollision(Vector3D(a, b, 0), 0))
				{
					return false;
				}
			}
		}
	}

	return true;
}

// Main collision check
// Checks all tiles the location + boundary intersect
// return:
//		0 : No Collision
//		1+ : UnitID of collision
int Collision::CheckUnitCollision(int nUnitID, const GameMath::Circle& nBoundary,bool nMoveUnit)
{
	mAvailableTiles.clear();
	int x, minX, maxX, y, minY, maxY;
	minX = (nBoundary.mLocation.x - nBoundary.mRadius)/8;
	maxX = (nBoundary.mLocation.x + nBoundary.mRadius)/8;
	minY = (nBoundary.mLocation.y - nBoundary.mRadius)/8;
	maxY = (nBoundary.mLocation.y + nBoundary.mRadius)/8;
	for (int a = minX; a <= maxX; a++)
	{
		for (int b = minY; b <= maxY; b++)
		{
			std::pair<int, int> tileLoc(a, b);
			auto range = mTile2Unit.equal_range(tileLoc);
			for (auto it = range.first; it != range.second; ++it)
			{
				if (it->second != nUnitID)
				{
					if (mUnits.at(it->second)->collide(nBoundary))
					{
						return it->second;
					}
				}
			}
			if (nMoveUnit)
			{
				mAvailableTiles.push_back(tileLoc);
			}
		}
	}
	cleanID(nUnitID);
	for (int a = 0; a < mAvailableTiles.size(); a++)
	{
		mUnitID2Tile.insert(std::pair<int, pair<int, int>>(nUnitID, mAvailableTiles[a]));
		mTile2Unit.insert(std::pair<pair<int, int>,int>(mAvailableTiles[a],nUnitID));
	}

	return 0;
}

// Find all units in circle area
// nUnitsToFind) types are:
// 0 - All
// 1 - Player 
// 2 - Enemy
std::vector<shared_ptr<Unit>> Collision::getUnitsInRange(int nUnitID, const GameMath::Circle& nBoundary,int nPlayer, int nUnitsToFind)
{
	std::vector<shared_ptr<Unit>> unitsFound;
	std::vector<int> idFound;
	int x, minX, maxX, y, minY, maxY;
	minX = (nBoundary.mLocation.x - nBoundary.mRadius) / 8;
	maxX = (nBoundary.mLocation.x + nBoundary.mRadius) / 8;
	minY = (nBoundary.mLocation.y - nBoundary.mRadius) / 8;
	maxY = (nBoundary.mLocation.y + nBoundary.mRadius) / 8;
	for (int a = minX; a <= maxX; a++)
	{
		for (int b = minY; b <= maxY; b++)
		{
			std::pair<int, int> tileLoc(a, b);
			auto range = mTile2Unit.equal_range(tileLoc);
			for (auto it = range.first; it != range.second; ++it)
			{
				if (it->second != nUnitID)
				{
					if (mUnits.at(it->second)->collide(nBoundary))
					{
						int player = mUnits.at(it->second)->getPlayer();
						bool unitCheck = false;
						if (nUnitsToFind == 0)
						{
							unitCheck = true;
						}
						else if (nUnitsToFind == 1 && (mUnits.at(it->second)->getPlayer() == nPlayer))
						{
							unitCheck = true;
						}
						else if (nUnitsToFind == 2 && (mUnits.at(it->second)->getPlayer() != nPlayer))
						{
							unitCheck = true;
						}
						if (unitCheck == true)
						{
							if (mUnits.at(it->second)->isTargetable() == true && std::find(idFound.begin(),idFound.end(), it->second) == idFound.end())
							{
								idFound.push_back(it->second);
								unitsFound.push_back(mUnits.at(it->second));
							}
							
						}
						
					}
				}
			}
		}
	}
	return unitsFound;
}

// Bresenham's line algorithm
std::vector<Vector3D> Collision::TilesUnderLine(float x0, float y0, float x1, float y1)
{
	std::vector<Vector3D> tiles;
	double dx = fabs(x1 - x0);
	double dy = fabs(y1 - y0);

	int x = int(floor(x0));
	int y = int(floor(y0));

	int n = 1;
	int x_inc, y_inc;
	double error;

	if (dx == 0)
	{
		x_inc = 0;
		error = std::numeric_limits<double>::infinity();
	}
	else if (x1 > x0)
	{
		x_inc = 1;
		n += int(floor(x1)) - x;
		error = (floor(x0) + 1 - x0) * dy;
	}
	else
	{
		x_inc = -1;
		n += x - int(floor(x1));
		error = (x0 - floor(x0)) * dy;
	}

	if (dy == 0)
	{
		y_inc = 0;
		error -= std::numeric_limits<double>::infinity();
	}
	else if (y1 > y0)
	{
		y_inc = 1;
		n += int(floor(y1)) - y;
		error -= (floor(y0) + 1 - y0) * dx;
	}
	else
	{
		y_inc = -1;
		n += y - int(floor(y1));
		error -= (y0 - floor(y0)) * dx;
	}

	for (; n > 0; --n)
	{
		tiles.push_back(Vector3D(x, y,0));

		if (error > 0)
		{
			y += y_inc;
			error -= dx;
		}
		else
		{
			x += x_inc;
			error += dy;
		}
	}
	//const bool steep = (fabs(y2 - y1) > fabs(x2 - x1));
	//if (steep)
	//{
	//	std::swap(x1, y1);
	//	std::swap(x2, y2);
	//}

	//if (x1 > x2)
	//{
	//	std::swap(x1, x2);
	//	
	//}
	//if (y1 > y2)
	//{
	//	std::swap(y1, y2);
	//}

	//const float dx = x2 - x1;
	//const float dy = fabs(y2 - y1);

	//float error = dx / 2.0f;
	//const int ystep = (y1 < y2) ? 1 : -1;
	//int y = (int)y1;

	//const int maxX = (int)x2;

	//for (int x = (int)x1; x<=maxX; x++)
	//{
	//	if (steep)
	//	{
	//		tiles.push_back(Vector3D(y, x, 0));
	//	}
	//	else
	//	{
	//		tiles.push_back(Vector3D(x, y, 0));
	//	}

	//	error -= dy;
	//	if (error < 0)
	//	{
	//		y += ystep;
	//		error += dx;
	//	}
	//}
	return(tiles);
}

void Collision::cleanID(int nUnitID)
{
	auto range = mUnitID2Tile.equal_range(nUnitID);
	// Use unitID to find all tiles that contain unit;
	for (auto it = range.first; it != range.second; ++it)
	{
		// loop over multimap of tile locations removing the unitID;
		auto rangeTiles = mTile2Unit.equal_range(it->second);
		for (auto it2 = rangeTiles.first; it2 != rangeTiles.second; ++it2)
		{
			if (it2->second == nUnitID)
			{
				mTile2Unit.erase(it2);
				break;
			}
		}
	}
	mUnitID2Tile.erase(nUnitID);
}

Collision::~Collision()
{
}
