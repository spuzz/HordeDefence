#pragma once

#include "Unit.h"
#include "Tile.h"
#include "Circle.h"
#include <map>

class Collision
{
public:
	Collision(const std::map < std::pair< int, int >, shared_ptr<Tile> >& nTiles);
	~Collision();

	bool addUnit(shared_ptr<Unit> unit);
	void Collision::deleteUnit(const int& mUnitID);

	void tryMove();

	// Return unitID, -1 if no collision 
	int CheckUnitCollision(int nUnitID, const GameMath::Circle& nBoundary, bool nMoveUnit);

	// Check tiles are passable
	bool CheckTileCollision(const GameMath::Circle& nBoundary, bool nMoveUnit);

	// Find all units in range
	std::vector<shared_ptr<Unit>> getUnitsInRange(int nUnitID, const GameMath::Circle& nBoundary, int mPlayer, int nUnitsToFind);

	// Remove unitID from grid;
	void cleanID(int nUnitID);
	

private:
	std::map<int, std::shared_ptr<Unit>> mUnits;
	std::multimap<pair<int,int>,int> mTile2Unit;
	std::multimap<int, pair<int, int>> mUnitID2Tile;
	std::vector<pair<int, int>> mAvailableTiles;
	const std::map < std::pair< int, int >, shared_ptr<Tile> >& mTiles;
};

