#pragma once

#include <unordered_map>
#include <unordered_set>

#include "GameBase/Point.h"
#include "GameBase/GameObject.h"

struct GameObject;

struct HashPair {
	template <class T1, class T2>
	size_t operator()(const std::pair<T1, T2>& p) const
	{
		return (p.first * 0x1f1f1f1f) ^ p.second;
	}
};

using Tile = std::pair<int32_t, int32_t>;
using TileMap = std::unordered_map<Tile, std::unordered_set<GameObject*>, HashPair>;

struct PartitionGrid {
public:
	PartitionGrid(float physicalSize, uint32_t numTiles) : m_numTiles(numTiles), m_gridSize(physicalSize) {}
	void AddObject(GameObject* obj, float x, float y);
	void AddObject(GameObject* obj, Point p);
	void DeleteObject(GameObject* obj, Tile tile);
	uint32_t RemoveDisabled();

	Tile GetTile(float x, float y) const;
	Tile GetTile(Point p) const;
	int32_t NumTiles() const;
	std::vector<Tile> GetNeighboringTiles(Tile tile, uint32_t level) const;
	std::unordered_set<GameObject*>& GetObjsInTile(Tile tile);
	bool IsOffsetTileInsideBounds(Tile tile, int32_t offset_x, int32_t offset_y) const;

private:
	TileMap m_map{};
	int32_t m_numTiles;
	float m_gridSize;
};