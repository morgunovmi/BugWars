#include "pch.h"
#include "PartitionGrid.h"

#include <cassert>

void PartitionGrid::AddObject(GameObject* obj, float x, float y)
{
	m_map[GetTile(x, y)].insert(obj);
}

void PartitionGrid::DeleteObject(GameObject* obj, float x, float y)
{
	assert(GetObjsInTile(GetTile(x, y)).erase(obj) == 1 
		&& "Removing an object from tile but it's not there");
}

Tile PartitionGrid::GetTile(float x, float y)
{
	return { static_cast<uint32_t>(x / m_gridSize * m_numTiles),
		     static_cast<uint32_t>(y / m_gridSize * m_numTiles) };
}

std::unordered_set<GameObject*>& PartitionGrid::GetObjsInTile(Tile tile)
{
	return m_map[tile];
}