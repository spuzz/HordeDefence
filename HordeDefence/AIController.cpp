#include "AIController.h"
#include "tinyxml2.h"
#include <iostream>
#include "Model.h"

AIController::AIController(std::shared_ptr<Model> nModel, std::string mapName, std::vector<Vector3D>& nSpawnLocations, const int& nMapWidth, const int& nMapHeight ) : mModel(nModel), mSpawnLocations(nSpawnLocations)
{
	std::string fileName = "..\\HordeDefenceArt\\Maps\\AIController\\" + mapName + ".xml";
	mMapWidth = nMapWidth;
	mMapHeight = nMapHeight;
	ParseXML(fileName);
	mLastWaveSpawn = -1;
}


AIController::~AIController()
{
}

// For each frame determine if new wave needs to be spawned
void AIController::Update(float nGameTimeSeconds)
{
	if (mWaves.size() == 0)
	{
		return;
	}
	if (mWaves[0].mWaveTime <= nGameTimeSeconds)
	{
		for (auto unit : mWaves[0].mUnits)
		{
			std::vector<Vector3D> possibleLocations = GetPossibleLocations(mSpawnLocations[0]);
			for (int a = 0; a <= unit.second; a++) 
			{
				srand(time(NULL));
				int randomIndex = rand() % possibleLocations.size();
				int unitID;
				while ((unitID = mModel->addBasicUnit(mMapWidth - possibleLocations[randomIndex].x, mMapHeight - possibleLocations[randomIndex].y, 0, unit.first.first, 1, unit.first.second)) != -1)
				{
					mModel->getAllUnits()[unitID]->attackMove(Vector3D(64, 64, 0));
					randomIndex = rand() % possibleLocations.size();
				}
			}
			
		}
		mWaves.erase(mWaves.begin());
	}
}

// Return random spawn location
Vector3D AIController::GetNextSpawn()
{
	srand(time(NULL));
	int randomIndex = rand() % mSpawnLocations.size();
	while (randomIndex == mLastWaveSpawn)
	{
		randomIndex = rand() % mSpawnLocations.size();
	}
	mLastWaveSpawn = randomIndex;
	return mSpawnLocations.at(randomIndex);
}

// returns list of locations in 2 tile radius of spawn
std::vector<Vector3D> AIController::GetPossibleLocations(Vector3D spawn)
{
	std::vector<Vector3D> possibleLocations;
	for (int width = -2; width <= 2; width++)
	{
		for (int height = -2; height <= 2; height++)
		{
			Vector3D loc(spawn.x + float(width), spawn.y + float(height),0.0f);
			if (loc.x <= mMapWidth && loc.x >= 0 && loc.y <= mMapHeight && loc.y >= 0)
			{
				possibleLocations.push_back(loc);
			}
			
		}
	}
	return possibleLocations;
}

// Parse the map xml that contains all wave times and unit info
void AIController::ParseXML(std::string fileName)
{

	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(fileName.c_str()) == tinyxml2::XML_NO_ERROR)
	{
		tinyxml2::XMLElement* mainXMLElement = doc.FirstChildElement("AISpawnCtrl");
		tinyxml2::XMLElement* eventsElement = mainXMLElement->FirstChildElement("Events");
		tinyxml2::XMLElement* wavesElement = eventsElement->FirstChildElement("Wave");
		while (wavesElement != nullptr)
		{
			
			tinyxml2::XMLElement* timeElement = wavesElement->FirstChildElement("Time");
			Wave wave(atoi(timeElement->GetText()));
			tinyxml2::XMLElement* unitElement = wavesElement->FirstChildElement("Unit");
			while (unitElement != nullptr)
			{
				wave.AddUnit(unitElement->Attribute("type"), unitElement->Attribute("classType"), atoi(unitElement->Attribute("amount")));
				unitElement = unitElement->NextSiblingElement();			
			}
			
			mWaves.push_back(wave);
			
			wavesElement = wavesElement->NextSiblingElement();
		}
	}
}