#include "pch.h"
#include "Bug.h"
#include "Game.h"

IMPLEMENT_RTTI(Bug);

void Bug::OnUpdate(float dt)
{
	if (g_Game->grid.GetTile(position) != lastTile)
	{
		g_Game->grid.DeleteObject(this, lastTile);
		g_Game->grid.AddObject(this, position);
		lastTile = g_Game->grid.GetTile(position);
	}
}

BugBase* Bug::FindBugToEat() const
{
	Bug* target = nullptr;
	const auto tile = g_Game->grid.GetTile(position);
	int32_t window_size = 1;

	float min_dist = std::numeric_limits<float>::max();
	while (!target && !g_Game->grid.IsOutsideBounds(tile, window_size / 2))
	{
		int32_t dist_to_edge = window_size / 2;

		for (int32_t i = -dist_to_edge; i <= dist_to_edge; ++i)
		{
			for (const auto off : { -dist_to_edge, dist_to_edge })
			{
				if (!g_Game->grid.IsOffsetTileInsideBounds(tile, off, i))
					continue;

				for (auto object : g_Game->grid.GetObjsInTile({ tile.first + off, tile.second + i }))
				{
					auto* bug = static_cast<Bug*>(object);
					if (bug == this)
						continue;

					if (bug->disabled)
						continue;

					if (bug->id > id)
						continue; // Can't eat that

					float dist = position.Distance(bug->position);
					if (dist < min_dist)
					{
						min_dist = dist;
						target = bug;
					}
				}
			}
		}

		for (int32_t j = -dist_to_edge; j <= dist_to_edge; ++j)
		{
			for (const auto off : { -dist_to_edge, dist_to_edge })
			{
				if (!g_Game->grid.IsOffsetTileInsideBounds(tile, j, off))
					continue;

				for (auto object : g_Game->grid.GetObjsInTile({ tile.first + j, tile.second + off }))
				{
					auto* bug = static_cast<Bug*>(object);
					if (bug == this)
						continue;

					if (bug->disabled)
						continue;

					if (bug->id > id)
						continue; // Can't eat that

					float dist = position.Distance(bug->position);
					if (dist < min_dist)
					{
						min_dist = dist;
						target = bug;
					}
				}
			}
		}

		if (target && window_size > 1) return target;
		window_size += 2;
	}

	return target;
}

void Bug::OnEat(BugBase& first, BugBase& second)
{
	if (first.id > second.id)
	{
		second.disabled = true;
		second.visible = false;
	}
	else
	{
		first.disabled = true;
		first.visible = false;
	}
}
