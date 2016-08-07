#pragma once
#include <map>
#include "Tile.h"
#include "Unit.h"
#include <memory>
#include "TmxLoader\Tmx.h"
#include "textureLoader.h"
using std::map;
class Model
{
public:
	Model();
	virtual ~Model();

	void addTile(Tile inTile) { mTiles.insert(pair<int, Tile>(inTile.getID(), inTile)); mSprites.push_back(std::shared_ptr<Sprite>(&mTiles[inTile.getID()])); }
	map<int, Tile>& getTiles(void) { return mTiles; }

	void addPlayerUnit(std::shared_ptr<Unit> inUnit) { mPlayerUnits.push_back(inUnit); mSprites.push_back(std::shared_ptr<Sprite>(&*inUnit)); }

	virtual void update();
	void actionOnLocation(float x, float y);

	std::vector<std::shared_ptr<Unit>>& getPlayerUnits() { return mPlayerUnits; }

	std::vector < std::shared_ptr<Sprite>>& getSprites() { return mSprites; }

	std::shared_ptr<Tmx::Map> getTmxMap() { return tmxMap; }

protected:
	shared_ptr<gridVector> mAstarGrid;
private:

	void createMap();
	std::shared_ptr<Tmx::Map> tmxMap;
	map<int, Tile> mTiles;
	std::vector<std::shared_ptr<Unit>> mPlayerUnits;
	std::vector < std::shared_ptr<Sprite>> mSprites;
};

