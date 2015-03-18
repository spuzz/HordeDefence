#pragma once
#include <map>
#include "Tile.h"
#include "Unit.h"
#include <memory>
#include "TmxLoader\Tmx.h"
using std::map;
class Model
{
public:
	Model();
	virtual ~Model();

	void addTile(Tile inTile) { mTiles.insert(pair<int,Tile>(inTile.getID(),inTile)); }
	const map<int, Tile> getTiles(void) const { return mTiles; }

	void addEnemyUnit(std::shared_ptr<Unit> inUnit) { mEnemyUnits.push_back(inUnit); }
	void addPlayerUnit(std::shared_ptr<Unit> inUnit) { mPlayerUnits.push_back(inUnit); }

	virtual void update();
	void actionOnLocation(float x, float y);

	std::vector<std::shared_ptr<Unit>> getEnemyUnits() { return mEnemyUnits; }
	std::vector<std::shared_ptr<Unit>> getPlayerUnits() { return mPlayerUnits; }

	std::shared_ptr<Tmx::Map> getTmxMap() { return tmxMap; }

protected:
	shared_ptr<gridVector> mAstarGrid;
private:

	void createMap();
	std::shared_ptr<Tmx::Map> tmxMap;
	map<int, Tile> mTiles;
	std::vector<std::shared_ptr<Unit>> mPlayerUnits;
	std::vector<std::shared_ptr<Unit>> mEnemyUnits;
};

