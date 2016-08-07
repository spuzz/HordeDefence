#pragma once
#include "Tile.h"
class TerrainFeature :
	public Sprite
{
public:
	TerrainFeature();
	virtual ~TerrainFeature();

	virtual void addTile(const Tile& n_tile) { m_tiles.push_back(n_tile); }
	virtual const std::vector<Tile>& getTiles() { return m_tiles;  }
private:
	std::vector<Tile> m_tiles;
};

