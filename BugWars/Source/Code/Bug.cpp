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
	float min_dist = std::numeric_limits<float>::max();
	for (auto object : g_Game->objects)
	{
		if (object->GetRTTI() == Bug::s_RTTI)
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
