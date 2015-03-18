#include "worldCreation.h"


worldCreation::worldCreation(shared_ptr<Model> inModel)
{
	int tileCount = 0;
	for (int i = 0; i < 32; i++){
		for (int j = 0; j < 32; j++) {
			tileCount += 1;
			inModel->addTile(Tile(tileCount, j, i, 10, 10, true));
		}
	}
}


worldCreation::~worldCreation()
{
}
