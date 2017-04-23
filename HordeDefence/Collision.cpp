#include "Collision.h"



Collision::Collision(const std::map < std::pair< int, int >, shared_ptr<Tile> >& nTiles) : mTiles(nTiles)
{
}

bool Collision::addUnit(shared_ptr<Unit> nUnit)
{
	if (CheckUnitCollision(nUnit->getObjectID(), nUnit->getBoundingBox(), true) != 0)
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


bool Collision::CheckTileCollision(const GameMath::Circle& nBoundary, bool nMoveUnit)
{
	int x, minX, maxX, y, minY, maxY;
	minX = (nBoundary.mLocation.x - nBoundary.mRadius);
	maxX = (nBoundary.mLocation.x + nBoundary.mRadius);
	minY = (nBoundary.mLocation.y - nBoundary.mRadius);
	maxY = (nBoundary.mLocation.y + nBoundary.mRadius);
	for (int a = minX; a < maxX; a++)
	{
		for (int b = minY; b < maxY; b++)
		{
			if (GameMath::GameMath::circleToRect(nBoundary, GameMath::Rectangle(Vector3D(a, b, 0), 1, 1)))
			{
				std::pair<int, int> tileLoc(a, b);
				auto findTile = mTiles.find(tileLoc);
				if (findTile == mTiles.end() || findTile->second->isWalkable() == false)
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
	int x, minX, maxX, y, minY, maxY;
	minX = (nBoundary.mLocation.x - nBoundary.mRadius) / 8;
	maxX = (nBoundary.mLocation.x + nBoundary.mRadius) / 8;
	minY = (nBoundary.mLocation.y - nBoundary.mRadius) / 8;
	maxY = (nBoundary.mLocation.y + nBoundary.mRadius) / 8;
	for (int a = minX; a < maxX; a++)
	{
		for (int b = minY; b < maxY; b++)
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
							unitsFound.push_back(mUnits.at(it->second));
						}
						
					}
				}
			}
		}
	}
	return unitsFound;
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
