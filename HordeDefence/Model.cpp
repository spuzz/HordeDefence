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
	//mTiles[0] = Tile(0, 0, 0, 0, 0, 0);
	std::string fileName = "..\\HordeDefenceArt\\Maps\\DemoMap.tmx";
	tmxMap->ParseFile(fileName);
	createMap();

	fileName = "..\\HordeDefenceArt\\Units\\Units.xml";

	ParseUnitXml(fileName);
	worldCreation newWorld(this);
	mCollisionSytem = std::shared_ptr<Collision>(new Collision(mTiles));
	addHumanCharacter(45, 41, 3, 0, "greatstaff", "clothes", "", 0);
	addHumanCharacter(43, 42, 3, 0, "sword", "steel", "shield",0);
	addBasicUnit(46, 35, 3, 1,1);
	addBasicUnit(34, 34, 3, 1, 1);
	addTeleporter(45, 46, 3);
	mAllUnits.at(mGameObjectID - 1)->attackMove(Vector3D(50, 34,0));
	//mPlayerUnits.back()->SetEquipment("greatstaff", "clothes", "");
	//addPlayerUnit(std::shared_ptr<Unit>(new Unit(mAstarGrid, Vector3D(43, 42, 3), *mUnitTypes[0].get())));
	//mPlayerUnits.back()->SetEquipment("sword", "steel", "shield");
	//Knight unitTest(1, 100, 100, pair<float, float>(50, 50), 0);
	//mPlayerUnits.push_back(std::shared_ptr<Unit>(new Knight(mAstarGrid,1, 100, 100, Vect(45, 50, 0), 0)));
	//addPlayerUnit(std::shared_ptr<Unit>(new Knight(mAstarGrid, 1, 100, 100, Vect(40, 61, 3), 0)));
	//mPlayerUnits.push_back(std::shared_ptr<Unit>(new Knight(mAstarGrid,1, 100, 100, pair<float, float>(30, 70), 0)));
}



shared_ptr<Tile> Model::getTile(int x, int y)
{
	return mTiles[std::pair<int,int>(x, y)];
}

void Model::update(float nSeconds)
{
	if (nSeconds > 0.0166)
	{
		nSeconds = 0.0166;
	}

	std::vector<int> deletableIds;
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
	//for (auto unit : mPlayerUnits) {
	//	unit->newTarget(Vector3D(x, y, 0));
	//}
}

void Model::selectOnLocation(float x, float y)
{
	for (UnitMap::const_iterator it = mAllUnits.begin(); it != mAllUnits.end(); ++it)
	{
		if (it->second->collide(Vector3D(x, y, 0)))
		{
			for (auto unit : mSelectedUnits)
			{
				it->second->setSelected(false);
			}
			mSelectedUnits.clear();
			it->second->setSelected(true);
			mSelectedUnits.insert(pair<int, shared_ptr<Unit>>(it->second->getObjectID(),it->second));
			break;
		}
	}
}

// Determine if location clicked is a unit or tile and push action on selected units
void Model::actionOnLocation(float x, float y) 
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
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {
				const Tmx::MapTile tileTest = tmxMap->GetTileLayers()[i - 1]->GetTile(y, x);
				int gid = tmxMap->GetTileLayers()[i - 1]->GetTileId(y, x);
				int walkable = tmxMap->GetTileset(0)->GetTile(gid)->GetProperties().GetIntProperty("Walkable");
				if (tileTest.tilesetId == -1) {
					if (i == 0) {
						mAstarGrid->at(width - x - 1)[height - y - 1] = -1;
					}
					continue;
				}
				else if (mAstarGrid->at(width - x - 1)[height - y - 1] != 0) {
					continue;
				}	
				else {
					if (walkable == 1) {
						mAstarGrid->at(width - x - 1)[height - y - 1] = 10;
					}
					else if(walkable == 0) {
						mAstarGrid->at(width - x - 1)[height - y - 1] = -1;
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
		tinyxml2::XMLElement* unitElement = mainXMLElement->FirstChildElement("unit");
		while (unitElement != nullptr)
		{
			shared_ptr<UnitType> unit(new UnitType());
			mUnitTypes.push_back(unit);
			tinyxml2::XMLElement* element = unitElement->FirstChildElement("name");
			unit->setName(element->GetText());
			element = unitElement->FirstChildElement("movement_speed");
			unit->setMoveSpeed(std::stof(element->GetText()));
			element = unitElement->FirstChildElement("attack_speed");
			unit->setAttackSpeed(std::stof(element->GetText()));
			element = unitElement->FirstChildElement("size");
			unit->setSize(std::stof(element->GetText()));
			element = unitElement->FirstChildElement("hp");
			unit->setHitPoints(std::stof(element->GetText()));
			tinyxml2::XMLElement* animations = unitElement->FirstChildElement("animations");
			element = animations->FirstChildElement("animation");
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

void Model::addHumanCharacter(const float& xLoc, const float& yLoc, const float& zDepth, const int& nUnitType, const string& nWeapon, const string& nArmor, const string& nOffhand, const int& isPlayerUnit)
{
	mGameObjectID++;
	HumanCharacter unit(mAstarGrid, Vector3D(xLoc, yLoc, zDepth), *mUnitTypes[nUnitType].get(),mGameObjectID);
	unit.SetEquipment(nWeapon, nArmor, nOffhand);
	
	shared_ptr<Unit> newUnit = std::make_shared<HumanCharacter>(unit);
	addUnit(newUnit, isPlayerUnit);
	
}

void Model::addBasicUnit(const float& xLoc, const float& yLoc, const float& zDepth, const int& nUnitType,const int& isPlayerUnit)
{
	mGameObjectID++;
	Unit unit(mAstarGrid, Vector3D(xLoc, yLoc, zDepth), *mUnitTypes[nUnitType].get(), mGameObjectID);

	shared_ptr<Unit> newUnit = std::make_shared<Unit>(unit);
	addUnit(newUnit, isPlayerUnit);

}

void Model::addTeleporter(const float& xLoc, const float& yLoc, const float& zDepth)
{
	mGameObjectID++;
	Teleporter unit(mAstarGrid, Vector3D(xLoc, yLoc, zDepth), *mUnitTypes[2].get(), mGameObjectID);

	shared_ptr<Unit> newUnit = std::make_shared<Teleporter>(unit);
	addUnit(newUnit, 0);

}

void Model::addUnit(shared_ptr<Unit> nUnit, const int& isPlayerUnit)
{
	if (mCollisionSytem->addUnit(nUnit) == false)
	{
		return;
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
