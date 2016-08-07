#include "Model.h"
#include "Knight.h"

Model::Model() : tmxMap(new Tmx::Map()), mAstarGrid(new gridVector())
{
	//mTiles[0] = Tile(0, 0, 0, 0, 0, 0);
	std::string fileName = "C:\\GameDev\\HordeDefence\\HordeDefenceArt\\Maps\\DemoMap.tmx";
	tmxMap->ParseFile(fileName);
	createMap();

	//Knight unitTest(1, 100, 100, pair<float, float>(50, 50), 0);
	//mPlayerUnits.push_back(std::shared_ptr<Unit>(new Knight(mAstarGrid,1, 100, 100, Vect(45, 50, 0), 0)));
	addPlayerUnit(std::shared_ptr<Unit>(new Knight(mAstarGrid, 1, 100, 100, Vect(40, 61, 3), 0)));
	//mPlayerUnits.push_back(std::shared_ptr<Unit>(new Knight(mAstarGrid,1, 100, 100, pair<float, float>(30, 70), 0)));
}

void Model::update()
{
	for (auto playerUnit : getPlayerUnits())
	{
		playerUnit->update();
	}
}

void Model::actionOnLocation(float x, float y) 
{
	for (auto unit : mPlayerUnits) {
		unit->newTarget(pair<float,float>(x, y));
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
					if (walkable == 1 || walkable == 0) {
						mAstarGrid->at(width - x - 1)[height - y - 1] = 10;
					}
					else {
						mAstarGrid->at(width - x - 1)[height - y - 1] = -1;
					}
					
				}
			}
		}
	}

}
Model::~Model()
{
}
