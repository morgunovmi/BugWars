#pragma once

#include <unordered_map>
#include <unordered_set>

struct GameObject;

using Tile = std::pair<uint32_t, uint32_t>;

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

struct PartitionGrid {
public:
	PartitionGrid(float physicalSize, uint32_t numTiles) : m_numTiles(numTiles), m_gridSize(physicalSize) {}
	void AddObject(GameObject* obj, float x, float y);
    void DeleteObject(GameObject* obj, float x, float y);
	Tile GetTile(float x, float y);
	std::unordered_set<GameObject*>& GetObjsInTile(Tile tile);

private:
	std::unordered_map<Tile, std::unordered_set<GameObject*>, HashPair> m_map{};
	uint32_t m_numTiles;
	float m_gridSize;
};