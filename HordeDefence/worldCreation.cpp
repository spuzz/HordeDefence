#include "worldCreation.h"


worldCreation::worldCreation(Model* inModel)
{
	std::shared_ptr<Tmx::Map> map = inModel->getTmxMap();
	float width = map->GetWidth();
	float height = map->GetHeight();
	inModel->setMapHeight(height);
	inModel->setMapWidth(width);
	int tileCount = 0;
	const Tmx::Tileset *set = map->GetTileset(0);
	int widthInTiles = map->GetTileset(0)->GetImage()->GetWidth() / map->GetTileset(0)->GetTileWidth();
	int heightInTiles = map->GetTileset(0)->GetImage()->GetHeight() / map->GetTileset(0)->GetTileHeight();
	for (auto tileLayer : map->GetTileLayers()) {
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {
				
				int test = tileLayer->GetTileTilesetIndex(y, x);
				int gid = tileLayer->GetTileId(y, x);
				const Tmx::MapTile tileTest = tileLayer->GetTile(y, x);
				if (tileTest.tilesetId == -1) {
					continue;
				}
				tileCount++;
				float tileset_col = (gid % widthInTiles);
				float tileset_row = gid / widthInTiles;
				if (x == 52 && y == 35)
				{

					int trap = 0;
				}
				int walkable = map->GetTileset(0)->GetTile(gid)->GetProperties().GetIntProperty("Walkable");
				Tile tile(tileCount, GameMath::Vector3D(width - 1 -x, height - 1 - y, tileLayer->GetZOrder()), map->GetTileset(0)->GetTileWidth(), map->GetTileset(0)->GetTileHeight(), walkable);
				//tile.SetTexture(inView->getTextureLoader()->retrieveMapTexture());
				tile.setWidthInTiles(widthInTiles);
				tile.setHeightInTiles(heightInTiles);
				tile.setTileRow(tileset_row);
				tile.setTileCol(tileset_col);

				tile.setMapWidth(width);
				tile.setMapHeight(height);
				tile.setAABBModelSpace(Vector3D(-1, -1, 0), Vector3D(1, 1, 0));
				tile.calcAABBWorldSpace();
				tile.SetTilesetID(tileTest.tilesetId);
				
				inModel->addTile(tile);
			}

		}
	}
}


worldCreation::~worldCreation()
{
}
