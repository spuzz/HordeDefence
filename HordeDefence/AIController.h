#pragma once
#include <memory>
#include <string>
#include "Wave.h"
#include <vector>
#include "Vector3D.h"

class Model;

class AIController
{
public:
	AIController(std::shared_ptr<Model> nModel, std::string mapName, std::vector<GameMath::Vector3D>& nSpawnLocations, const int& nMapWidth, const int& nMapHeight);
	~AIController();

	void Update(float nGameTimeSeconds);

	int WavesLeft() { return mWaves.size(); }
	int TotalWaves() { return mTotalWaves;  }
	int SecondsToNextWave() { if (mWaves.size() > 0) { return mWaves[0].mWaveTime - mGameTime; } else { return 0; } }

private:

	void ParseXML(std::string fileName);

	std::shared_ptr<Model> mModel;

	std::vector<Wave> mWaves;
	std::vector<GameMath::Vector3D>& mSpawnLocations;

	int mTotalWaves;
	int mLastWaveSpawn;
	int mMapWidth;
	int mMapHeight;
	float mGameTime;
	GameMath::Vector3D GetNextSpawn();
	std::vector<GameMath::Vector3D> GetPossibleLocations(GameMath::Vector3D spawn);

};

