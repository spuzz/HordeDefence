#include "Model.h"
#include "Knight.h"
#include "tinyxml2.h"
#include "UnitType.h"
#include <string>     // std::string, std::stof
#include <utility> 
#include "worldCreation.h"
#include "Teleporter.h"


Model::Model() : tmxMap(new Tmx::Map()), mAstarGrid(new gridVector()) 
{
	std::string fileName = "..\\HordeDefenceArt\\Maps\\MapOne.tmx";
	tmxMap->ParseFile(fileName);
	createMap();

	fileName = "..\\HordeDefenceArt\\Units\\Units.xml";
	mCurrentTime = 0;
	ParseUnitXml(fileName);
	worldCreation newWorld(this);
	mCollisionSytem = std::shared_ptr<Collision>(new Collision(mTiles));

	mAIController = new AIController(std::shared_ptr<Model>(this),"MapOne", mSpawnLocations,mMapWidth,mMapHeight);
	//addHumanCharacter(45, 41, 3, "Human", "greatstaff", "clothes", "", 0,"Knight");
	//addHumanCharacter(43, 42, 3, 0, "sword", "steel", "shield",0);
	//addBasicUnit(46, 35, 3, 1,1);
	//addBasicUnit(34, 34, 3, "Orc", 1, "Warrior");
	//addTeleporter(45, 46, 3);

	mSelectionChanged = false;


}



shared_ptr<Tile> Model::getTile(int x, int y)
{
	return mTiles[std::pair<int,int>(x, y)];
}

std::vector<string> Model::getUnitTypes()
{
	std::vector<string> keys;
	for (std::map<string, std::map<string, shared_ptr<UnitType>>>::iterator it = mUnitTypes.begin(); it != mUnitTypes.end(); ++it) {
		keys.push_back(it->first);
	}
	return keys;
}

std::vector<string> Model::getUnitTypeClasses(const string& nUnitType)
{
	std::map<string, shared_ptr<UnitType>> unitClass = mUnitTypes[nUnitType];
	std::vector<string> keys;
	for(std::map<string, shared_ptr<UnitType>>::iterator it = unitClass.begin(); it != unitClass.end(); ++it) {
		keys.push_back(it->first);
	}
	return keys;
}

UnitType Model::getUnitType(const string& nUnitType, const string& nUnitClass)
{
	return *mUnitTypes[nUnitType][nUnitClass].get();
}

void Model::update(float nSeconds)
{
	if (nSeconds > 0.0166)
	{
		nSeconds = 0.0166;
	}

	mCurrentTime += nSeconds;
	std::vector<int> deletableIds;
	mAIController->Update(mCurrentTime);
	for (UnitMap::const_iterator it = mAllUnits.begin(); it != mAllUnits.end(); ++it)
	{
		if (it->second->isDead())
		{
			deletableIds.push_back(it->second->getObjectID());
		}
		else
		{
			it->second->update(nSeconds);
		}
		
	}

	for (auto id : deletableIds)
	{
		deleteUnit(id);
	}
}

void Model::selectOnLocation(GameMath::Rectangle rect)
{
	mSelectionChanged = true;
	for (auto unit : mSelectedUnits)
	{
		unit.second->setSelected(false);
	}
	mSelectedUnits.clear();
	for (auto unit : mPlayerUnits) {
		GameMath::Rectangle unitRect(Vector3D(unit.second->getScreenLocation().x - 2, unit.second->getScreenLocation().y - 1,0), 4, 4);
		if (GameMath::GameMath::rectToRect(unitRect,rect))
		{
			unit.second->setSelected(true);
			mSelectedUnits.insert(pair<int, shared_ptr<Unit>>(unit.second->getObjectID(), unit.second));
		}
	}
}

void Model::selectOnLocation(float x, float y)
{
	for (UnitMap::const_iterator it = mAllUnits.begin(); it != mAllUnits.end(); ++it)
	{
		GameMath::Rectangle unitRect(Vector3D(it->second->getScreenLocation().x - 2, it->second->getScreenLocation().y - 1, 0), 4, 4);
		if (GameMath::GameMath::pointToRect(Vector3D(x, y, 0), unitRect))
		{
			mSelectionChanged = true;
			mEnemySelected = false;
			for (auto unit : mSelectedUnits)
			{
				unit.second->setSelected(false);
			}
			mSelectedUnits.clear();
			it->second->setSelected(true);

			// Check if Player unit selected
			if (it->second->getPlayer() != 0)
			{
				mEnemySelected = true;
			}
			mSelectedUnits.insert(pair<int, shared_ptr<Unit>>(it->second->getObjectID(),it->second));
			break;
		}
	}
}

void Model::selectUnit(int unit)
{
	for (auto unit : mSelectedUnits)
	{
		unit.second->setSelected(false);
	}
	mSelectedUnits.clear();
	mAllUnits[unit]->setSelected(true);
	mSelectedUnits.insert(pair<int, shared_ptr<Unit>>(unit, mAllUnits[unit]));
	mSelectionChanged = true;
}

// Determine if location clicked is a unit or tile and push action on selected units
void Model::actionOnLocation(float x, float y) 
{
	// If no unit selected or an enemy is the selection do not do any action checks
	if (mSelectedUnits.size() != 0 && mEnemySelected == false)
	{
		for (UnitMap::const_iterator it = mAllUnits.begin(); it != mAllUnits.end(); ++it)
		{
			if (it->second->collide(Vector3D(x, y, 0)))
			{
				for (UnitMap::const_iterator itSelected = mSelectedUnits.begin(); itSelected != mSelectedUnits.end(); ++itSelected)
				{
					itSelected->second->newTarget(it->second);
				}
				// can't click on tile if unit found firsts so return
				return;
			}
		}
		if (getTile(x, y)->isWalkable())
		{
			for (UnitMap::const_iterator itSelected = mSelectedUnits.begin(); itSelected != mSelectedUnits.end(); ++itSelected)
			{
				itSelected->second->newTarget(Vector3D(x, y, 0));
			}
		}
	}


}
void Model::createMap()
{
	float width = tmxMap->GetWidth();
	float height = tmxMap->GetHeight();
	for (int a = 0; a < width; a++) {
		std::vector<int> v(height);
		std::fill(v.begin(), v.end(), 0);

		mAstarGrid->push_back(v);
	}


	for (int i = tmxMap->GetTileLayers().size(); i >=  1; i--) {
		if(tmxMap->GetTileLayers()[i - 1]->GetName() == "Collision") {
			for (int x = 0; x < width; x++) {
				for (int y = 0; y < height; y++) {
					const Tmx::MapTile tileTest = tmxMap->GetTileLayers()[i - 1]->GetTile(y, x);
					int gid = tmxMap->GetTileLayers()[i - 1]->GetTileId(y, x);
					int walkable = tmxMap->GetTileset(0)->GetTile(gid)->GetProperties().GetIntProperty("Walkable");
					if (walkable != 1)
					{
						mAstarGrid->at(width - x - 1)[height - y - 1] = -1;
					}
					else
					{
						mAstarGrid->at(width - x - 1)[height - y - 1] = 10;
					}
				}
			}
		}
		else if (tmxMap->GetTileLayers()[i - 1]->GetName() == "Spawn Points") {
			for (int x = 0; x < width; x++) {
				for (int y = 0; y < height; y++) {
					const Tmx::MapTile tileTest = tmxMap->GetTileLayers()[i - 1]->GetTile(y, x);
					int gid = tmxMap->GetTileLayers()[i - 1]->GetTileId(y, x);
					int spawnPoint = tmxMap->GetTileset(0)->GetTile(gid)->GetProperties().GetIntProperty("Spawn Point");
					if (spawnPoint != 0)
					{
						mSpawnLocations.push_back(Vector3D(x, y, 0));
					}
				}
			}
		}
	}

}


void Model::ParseUnitXml(const string& inFileName)
{

	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(inFileName.c_str()) == tinyxml2::XML_NO_ERROR)
	{
		tinyxml2::XMLElement* mainXMLElement = doc.FirstChildElement("horde_defence")->FirstChildElement("units");
		tinyxml2::XMLElement* unitsElement = mainXMLElement->FirstChildElement("unit");
		while (unitsElement != nullptr)
		{
			tinyxml2::XMLElement* unitElement = unitsElement->FirstChildElement("class");
			std::map < string, shared_ptr<UnitType>> units;
			
			while (unitElement != nullptr)
			{

				shared_ptr<UnitType> unit(new UnitType());
				unit->setName(unitsElement->Attribute("value"));
				unit->setClass(unitElement->Attribute("value"));
				tinyxml2::XMLElement* element = unitElement->FirstChildElement("movement_speed");
				unit->setMoveSpeed(std::stof(element->GetText()));
				element = unitElement->FirstChildElement("attack_speed");
				unit->setAttackSpeed(std::stof(element->GetText()));
				element = unitElement->FirstChildElement("size");
				unit->setSize(std::stof(element->GetText()));
				element = unitElement->FirstChildElement("hp");
				unit->setHitPoints(std::stof(element->GetText()));
				element = unitElement->FirstChildElement("range");
				unit->setRange(std::stof(element->GetText()));
				tinyxml2::XMLElement* animations = unitElement->FirstChildElement("animations");
				element = animations->FirstChildElement("animation");
				units[unit->getClass()] = unit;
				string name;
				int initFrame, frames;
				Animation anim;
				while (element != nullptr)
				{


					tinyxml2::XMLElement* nameElem = element->FirstChildElement("name");
					name = nameElem->GetText();
					tinyxml2::XMLElement* initElem = element->FirstChildElement("start");
					initFrame = atoi(initElem->GetText());
					tinyxml2::XMLElement* framesElem = element->FirstChildElement("frames");
					frames = atoi(framesElem->GetText());
					anim.addAnimation(name, initFrame, frames);
					element = element->NextSiblingElement();
				}
				unit->setAnimation(anim);
				unitElement = unitElement->NextSiblingElement();
			}
			mUnitTypes[unitsElement->Attribute("value")] = units;
			unitsElement = unitsElement->NextSiblingElement();
		}
	}

	
}

void Model::addTile(Tile inTile)
{
	mGameObjectID++;
	std::pair<int, int> loc(inTile.getIsoLocation().x, inTile.getIsoLocation().y);
	shared_ptr<Tile> tile(new Tile(inTile));
	if (mTiles.find(loc) != mTiles.end())
	{
		mTiles[loc] = tile;
	}
	else
	{
		mTiles.insert(std::pair< std::pair<int, int>, shared_ptr<Tile> >(loc, tile));

	}

	mGameObjects.push_back(tile);
}

int Model::addHumanCharacter(const float& xLoc, const float& yLoc, const float& zDepth, const string& nUnitType, const string& nWeapon, const string& nArmor, const string& nOffhand, const int& isPlayerUnit, const string& nUnitClass)
{
	if (mUnitTypes.find(nUnitType) == mUnitTypes.end())
	{
		return -1;
	}

	if (mUnitTypes[nUnitType].find(nUnitClass) == mUnitTypes[nUnitType].end())
	{
		return -1;
	}

	mGameObjectID++;
	HumanCharacter unit(mAstarGrid, Vector3D(xLoc, yLoc, zDepth), *mUnitTypes[nUnitType][nUnitClass].get(),mGameObjectID);
	unit.SetEquipment(nWeapon, nArmor, nOffhand);
	
	shared_ptr<Unit> newUnit = std::make_shared<HumanCharacter>(unit);
	if (addUnit(newUnit, isPlayerUnit) == true)
	{
		return mGameObjectID;
	}
	else
	{
		return -1;
	} 
	
}

int Model::addBasicUnit(const float& xLoc, const float& yLoc, const float& zDepth, const string& nUnitType,const int& isPlayerUnit, const string& nUnitClass)
{
	if (mUnitTypes.find(nUnitType) == mUnitTypes.end())
	{
		return -1;
	}

	if (mUnitTypes[nUnitType].find(nUnitClass) == mUnitTypes[nUnitType].end())
	{
		return -1;
	}
	mGameObjectID++;
	Unit unit(mAstarGrid, Vector3D(xLoc, yLoc, zDepth), *mUnitTypes[nUnitType][nUnitClass].get(), mGameObjectID);

	shared_ptr<Unit> newUnit = std::make_shared<Unit>(unit);
	if (addUnit(newUnit, isPlayerUnit) == true)
	{
		return mGameObjectID;
	}
	else
	{
		return -1;
	}

}

bool Model::addTeleporter(const float& xLoc, const float& yLoc, const float& zDepth)
{
	mGameObjectID++;
	Teleporter unit(mAstarGrid, Vector3D(xLoc, yLoc, zDepth), *mUnitTypes["Teleporter"][""].get(), mGameObjectID);

	shared_ptr<Unit> newUnit = std::make_shared<Teleporter>(unit);
	return addUnit(newUnit, 0);
	

}

bool Model::addUnit(shared_ptr<Unit> nUnit, const int& isPlayerUnit)
{
	if (mCollisionSytem->addUnit(nUnit) == false)
	{
		return false;
	}
	nUnit->setPlayer(isPlayerUnit);
	nUnit->setCollisionSystem(mCollisionSytem);
	mGameObjects.push_back(nUnit);
	mAllUnits.insert(pair<int, shared_ptr<Unit>>(mGameObjectID, nUnit));
	if (isPlayerUnit == 0)
	{
		mPlayerUnits.insert(pair<int, shared_ptr<Unit>>(mGameObjectID, nUnit));
	}
	else
	{
		mAIUnits.insert(pair<int, shared_ptr<Unit>>(mGameObjectID, nUnit));
	}
	return true;
}

void Model::deleteUnit(int nUnitID)
{
	std::vector<shared_ptr<GameObject>>::iterator itr = mGameObjects.begin();
	while (itr != mGameObjects.end()) {
		if ((*itr)->getObjectID() == nUnitID) {
			itr = mGameObjects.erase(itr);
		}
		else {
			++itr;
		}
	}
	mAllUnits.erase(nUnitID);
	mPlayerUnits.erase(nUnitID);
	mAIUnits.erase(nUnitID);
	mCollisionSytem->cleanID(nUnitID);
}


Model::~Model()
{
}
