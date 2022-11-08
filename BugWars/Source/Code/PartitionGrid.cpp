#include "pch.h"
#include "PartitionGrid.h"

#include "GameBase/Log.h"

#include <cassert>

void PartitionGrid::AddObject(GameObject* obj, float x, float y)
{
	const auto tile = GetTile(x, y);
	m_map[tile.second][tile.first].insert(obj);
}

void PartitionGrid::AddObject(GameObject* obj, Point p)
{
	AddObject(obj, p.x, p.y);
}

void PartitionGrid::DeleteObject(GameObject* obj, Tile tile)
{
	const auto removed = GetObjsInTile(tile).erase(obj);
	assert(removed == 1 
		&& "Removing an object from tile but it's not there");
}

int32_t PartitionGrid::NumTiles() const { return numTiles; }

// TODO Can pass a functor so that we return the object directly
std::vector<Tile> PartitionGrid::GetNeighboringTiles(Tile tile, uint32_t level) const
{
	// TODO no need to use heap, can just allocate a big array
	std::vector<Tile> neighbors{};
	if (level == 0) return { tile };

	auto offset = static_cast<int32_t>(level);

	// Vertical edges of the window
	for (int32_t i = -offset; i <= offset; ++i)
	{
		for (const auto& o : { -offset, offset })
		{
			if (IsOffsetTileInsideBounds(tile, o, i))
			{
				neighbors.emplace_back(Tile{ tile.first + o, tile.second + i });
			}
		}
	}

	// Horizontal edges of the window
	for (int32_t j = -offset + 1; j < offset; ++j)
	{
		for (const auto& o : { -offset, offset })
		{
			if (IsOffsetTileInsideBounds(tile, j, o))
			{
				neighbors.emplace_back(Tile{ tile.first + j, tile.second + o });
			}
		}
	}

	return neighbors;
}

Tile PartitionGrid::GetTile(float x, float y) const
{
	return { std::clamp(static_cast<int32_t>(x / physicalSize * numTiles), 0, numTiles),
		     std::clamp(static_cast<int32_t>(y / physicalSize * numTiles), 0, numTiles) };
}

Tile PartitionGrid::GetTile(Point p) const
{
	return GetTile(p.x, p.y);
}

std::unordered_set<GameObject*>& PartitionGrid::GetObjsInTile(Tile tile)
{
	return m_map[tile.second][tile.first];
}

bool PartitionGrid::IsOffsetTileInsideBounds(Tile tile, int32_t offset_x, int32_t offset_y) const
{
	return tile.first + offset_x >= 0 && tile.first + offset_x < numTiles
		&& tile.second + offset_y >= 0 && tile.second + offset_y < numTiles;
}