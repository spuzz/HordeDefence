#pragma once

#include "GameObject.h"

class Tile : public GameObject
{
public:
	Tile();
	Tile(int inID,int inxLoc,int inyLoc, int inWidth, int inHeight,bool inWalkable);
	Tile(int inID, Vector3D location, int inWidth, int inHeight,bool inWalkable);
	virtual ~Tile();

	int const getID(void) const { return mID; }
	int const getTilesetID(void) const { return tileSetId; }
	void SetTilesetID(int n_tilesetID) { tileSetId = n_tilesetID;  }
	int const getWidth(void) { return mWidth; }
	int const getHeight(void) { return mHeight;  }
	void setWidth(int inWidth) { mWidth = inWidth;  }
	void setheight(int inHeight) { mWidth = inHeight; }

	int const getMapWidth(void) { return mapWidth; }
	int const getMapHeight(void) { return mapHeight; }
	void setMapWidth(int n_mapWidth) { mapWidth = n_mapWidth; }
	void setMapHeight(int n_mapHeight) { mapHeight = n_mapHeight; }

	int const getWidthInTiles(void) { return widthInTiles; }
	int const getHeightInTiles(void) { return heightInTiles; }
	void setWidthInTiles(int n_widthInTiles) { widthInTiles = n_widthInTiles; }
	void setHeightInTiles(int n_heightInTiles) { heightInTiles = n_heightInTiles; }

	float const getTileRow(void) { return tileset_row; }
	float const getTileCol(void) { return tileset_col; }
	void setTileRow(int n_tileset_row) { tileset_row = n_tileset_row; }
	void setTileCol(int n_tileset_col) { tileset_col = n_tileset_col; }

	bool isWalkable() { return mWalkable; }

	void draw(const float& xScreenLoc, const float& yScreenLoc, const float& zScreenLoc, textureLoader& txtrLoader);

	bool collide(const GameMath::Circle& circle);
	bool collide(const GameMath::Rectangle& rect);
	bool collide(const GameMath::Vector3D& point);

	void update(float nSeconds) {};
protected:
	int mapWidth, mapHeight;

	int widthInTiles, heightInTiles;
	float tileset_row, tileset_col;
	int tileSetId;

	GameMath::Rectangle mBoundingBox;
private:
	int mID, mWidth, mHeight;
	bool mWalkable;

};

