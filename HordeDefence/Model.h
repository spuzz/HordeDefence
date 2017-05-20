#pragma once
#include <map>
#include "Tile.h"
#include "Unit.h"
#include "HumanCharacter.h"
#include <memory>
#include "TmxLoader\Tmx.h"
#include "textureLoader.h"
#include "UnitType.h"
#include "GameMath.h"
#include "Collision.h"

using std::map;



class Model
{
public:
	typedef map<int, shared_ptr<Unit>> UnitMap;
	typedef map<int, shared_ptr<GameObject>> GameObjectMap;
	Model();
	virtual ~Model();

	
	shared_ptr<Tile> getTile(int x, int y);
	map<std::pair< int, int >, shared_ptr<Tile>>& getTiles(void) { return mTiles; }

	void addTile(Tile inTile);
	void addUnit(shared_ptr<Unit> nUnit, const int& isPlayerUnit);
	void addHumanCharacter(const float& xLoc, const float& yLoc, const float& zDepth, const int& nUnitType, const string& nWeapon, const string& nArmor, const string& nOffhand, const int& isPlayerUnit);
	void addBasicUnit(const float& xLoc, const float& yLoc, const float& zDepth, const int& nUnitType, const int& isPlayerUnit);
	void addTeleporter(const float& xLoc, const float& yLoc, const float& zDepth);
	void deleteUnit(int nUnitID);

	virtual void update(float nSeconds);
	void actionOnLocation(float x, float y);
	void selectOnLocation(float x, float y);
	void selectOnLocation(GameMath::Rectangle rect);
	void selectUnit(int unit);
	void ParseUnitXml(const string& inFileName);

	std::map<int, std::shared_ptr<Unit>>& getPlayerUnits() { return mPlayerUnits; }

	std::vector<std::shared_ptr<GameObject>>& getGameObjects() { return mGameObjects; }

	std::map<int, std::shared_ptr<Unit>>& getAllUnits() { return mAllUnits; }

	std::map<int, std::shared_ptr<Unit>>& getAIUnits() { return mAIUnits; }

	std::map<int, std::shared_ptr<Unit>>& getSelectedUnits() { return mSelectedUnits; }

	std::shared_ptr<Tmx::Map> getTmxMap() { return tmxMap; }

	bool getSelectionChanged() { return mSelectionChanged; }
	void setSelectionChanged(const bool& nSel) { mSelectionChanged = nSel;  }

	void setMapWidth(int nMapWidth) { mMapWidth = nMapWidth; }
	void setMapHeight(int nMapHeight) { mMapHeight = nMapHeight; }

	int getMapWidth() { return mMapWidth; }
	int getMapHeight() { return mMapHeight; }
protected:
	shared_ptr<gridVector> mAstarGrid;
private:

	void createMap();
	std::shared_ptr<Tmx::Map> tmxMap;
	map < std::pair< int, int >, shared_ptr<Tile> > mTiles;
	std::map<int, std::shared_ptr<Unit>> mPlayerUnits;
	std::map<int, std::shared_ptr<Unit>> mAIUnits;
	std::map<int, std::shared_ptr<Unit>> mAllUnits;
	std::map<int, std::shared_ptr<Unit>> mSelectedUnits;
	std::vector<std::shared_ptr<GameObject>> mGameObjects;
	std::vector<shared_ptr<UnitType>> mUnitTypes;

	int mMapWidth;
	int mMapHeight;
	// Game Object Counter
	int mGameObjectID;

	// stop actions on enemy selection
	bool mEnemySelected;

	// Allow view to know selection has been updated
	bool mSelectionChanged;
	// delta time 
	double currentTime;

	// System modules
	std::shared_ptr<Collision> mCollisionSytem;
};

