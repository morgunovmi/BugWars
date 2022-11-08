#include "pch.h"
#include "Bug.h"
#include "Game.h"

IMPLEMENT_RTTI(Bug);

// TODO Maybe move update to grid class
void Bug::OnUpdate(float dt)
{
	if (g_Game->grid.GetTile(position) != lastTile)
	{
		g_Game->grid.DeleteObject(this, lastTile);
		g_Game->grid.AddObject(this, position);
		lastTile = g_Game->grid.GetTile(position);
	}
}

// TODO move same search implementation to grid class maybe
BugBase* Bug::FindBugToEat() const
{
	PIXScopedEvent(PIX_COLOR_INDEX(5), __FUNCTION__);
	Bug* target = nullptr;
	const auto tile = g_Game->grid.GetTile(position);
	uint32_t level = 0;

	float min_dist = std::numeric_limits<float>::max();
	while (!target)
	{
		const auto neighbors = g_Game->grid.GetNeighboringTiles(tile, level);
		if (neighbors.empty()) break;

		for (const auto& tile : neighbors)
		{
			for (auto& object : g_Game->grid.GetObjsInTile(tile))
			{
				auto* bug = static_cast<Bug*>(object);
				if (bug == this)
					continue;

				if (bug->disabled)
					continue;

				if (bug->id > id)
					continue; // Can't eat that

				float dist = position.Distance2(bug->position);
				if (dist < min_dist)
				{
					min_dist = dist;
					target = bug;
				}
			}
		}

		if (target && level > 0) return target;
		++level;
	}

	return target;
}

void Bug::OnEat(BugBase& first, BugBase& second)
{
	if (first.id > second.id)
	{
		second.disabled = true;
		second.visible = false;
		g_Game->grid.DeleteObject(&second, g_Game->grid.GetTile(second.position));
	}
	else
	{
		first.disabled = true;
		first.visible = false;
		g_Game->grid.DeleteObject(&first, g_Game->grid.GetTile(first.position));
	}
}
