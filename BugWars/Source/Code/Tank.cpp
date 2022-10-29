#include "pch.h"
#include "Tank.h"
#include "Globals.h"
#include "Game.h"
#include "Bullet.h"
#include "Bug.h"

IMPLEMENT_RTTI(Tank);

void Tank::OnUpdate(float dt)
{
}

BugBase* Tank::GetBugToShoot() const
{
	for (auto obj : g_Game->objects)
		if (obj->GetRTTI() == Bug::s_RTTI)
		{
			auto bug = static_cast<Bug*>(obj);
			return bug;
		}

	return nullptr;
}

Point Tank::CalcShootDirection(Point target_pos, Point target_dir, float target_vel, float bullet_vel) const
{
	return { 1, 0 };
}
