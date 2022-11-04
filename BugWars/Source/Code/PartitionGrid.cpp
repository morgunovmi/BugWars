#include "pch.h"
#include "PartitionGrid.h"

#include "GameBase/Log.h"

#include <cassert>

void PartitionGrid::AddObject(GameObject* obj, float x, float y)
{
	m_map[GetTile(x, y)].insert(obj);
}

void PartitionGrid::AddObject(GameObject* obj, Point p)
{
	AddObject(obj, p.x, p.y);
}

void PartitionGrid::DeleteObject(GameObject* obj, Tile tile)
{
	assert(GetObjsInTile(tile).erase(obj) == 1 
		&& "Removing an object from tile but it's not there");
}

int32_t PartitionGrid::NumTiles() const { return m_numTiles; }

Tile PartitionGrid::GetTile(float x, float y) const
{
	return { std::clamp(static_cast<int32_t>(x / m_gridSize * m_numTiles), 0, m_numTiles),
		     std::clamp(static_cast<int32_t>(y / m_gridSize * m_numTiles), 0, m_numTiles) };
}

Tile PartitionGrid::GetTile(Point p) const
{
	return GetTile(p.x, p.y);
}

std::unordered_set<GameObject*>& PartitionGrid::GetObjsInTile(Tile tile)
{
	return m_map[tile];
}

bool PartitionGrid::IsOffsetTileInsideBounds(Tile tile, int32_t offset_x, int32_t offset_y) const
{
	return tile.first + offset_x >= 0 && tile.first + offset_x < m_numTiles
		&& tile.second + offset_y >= 0 && tile.second + offset_y < m_numTiles;
}

bool PartitionGrid::IsOutsideBounds(Tile tile, int32_t dist_to_window_edge) const
{
	return tile.first + dist_to_window_edge >= m_numTiles && tile.first - dist_to_window_edge < 0 &&
		tile.second + dist_to_window_edge >= m_numTiles && tile.second - dist_to_window_edge < 0;
}
