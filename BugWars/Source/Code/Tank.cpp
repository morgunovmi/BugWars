#include "pch.h"
#include "Tank.h"
#include "Globals.h"
#include "Game.h"
#include "Bullet.h"
#include "Bug.h"

#include <cassert>
#include "GameBase/Log.h"

IMPLEMENT_RTTI(Tank);

void Tank::OnUpdate(float dt)
{
}

BugBase* Tank::GetBugToShoot() const
{
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
			for (auto object : g_Game->grid.GetObjsInTile(tile))
			{
				auto* bug = static_cast<Bug*>(object);

				if (bug->disabled)
					continue;

				float dist = position.Distance(bug->position);
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

Point Tank::CalcShootDirection(Point target_pos, Point target_dir, float target_vel, float bullet_vel) const
{
	const auto vec_to_target = target_pos - position;
	const auto a = target_vel * target_vel * target_dir.Length2() - bullet_vel * bullet_vel;
	const auto b = 2 * target_vel * target_dir.Dot(vec_to_target);
	const auto c = vec_to_target.Length2();

	const auto p = -b / (2 * a);
	const auto q = std::sqrtf((b * b) - 4 * a * c) / (2 * a);

	const auto t1 = p - q;
	const auto t2 = p + q;
	float t = 0.f;

	if (t1 > t2 && t2 > 0.f)
	{
		t = t2;
	}
	else
	{
		t = t1;
	}

	const auto bug_final_pos = target_pos + target_dir * target_vel * t;
	return (bug_final_pos - position).Normalized();
}
