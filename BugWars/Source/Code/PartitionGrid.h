#pragma once

#include <unordered_map>
#include <unordered_set>

#include "GameBase/Point.h"

struct GameObject;

struct HashPair {
    template <class T1, class T2>
    size_t operator()(const std::pair<T1, T2>& p) const
    {
        auto hash1 = std::hash<T1>{}(p.first);
        auto hash2 = std::hash<T2>{}(p.second);

        if (hash1 != hash2) {
            return hash1 ^ hash2;
        }

        return hash1;
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

	Tile GetTile(float x, float y) const;
    Tile GetTile(Point p) const;
    int32_t NumTiles() const;
    std::vector<Tile> GetNeighboringTiles(Tile tile, uint32_t level);
	std::unordered_set<GameObject*>& GetObjsInTile(Tile tile);
    bool IsOffsetTileInsideBounds(Tile tile, int32_t offset_x, int32_t offset_y) const;

	TileMap m_map{};
	int32_t m_numTiles;
	float m_gridSize;
};