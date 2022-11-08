#pragma once

#include <array>
#include <unordered_set>

#include "GameBase/Point.h"
#include "GameBase/GameObject.h"

struct GameObject;

constexpr float physicalSize = 9600.f;
constexpr int32_t numTiles = 10;

using Tile = std::pair<int32_t, int32_t>;
using TileMap = std::array<std::array<std::unordered_set<GameObject*>, numTiles>, numTiles>;

struct PartitionGrid {
public:
	PartitionGrid() {}
	void AddObject(GameObject* obj, float x, float y);
	void AddObject(GameObject* obj, Point p);
	void DeleteObject(GameObject* obj, Tile tile);

	Tile GetTile(float x, float y) const;
	Tile GetTile(Point p) const;
	int32_t NumTiles() const;

	std::vector<Tile> GetNeighboringTiles(Tile tile, uint32_t level) const;
	std::unordered_set<GameObject*>& GetObjsInTile(Tile tile);
	bool IsOffsetTileInsideBounds(Tile tile, int32_t offset_x, int32_t offset_y) const;

private:
	TileMap m_map{};
};