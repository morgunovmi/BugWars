#include "pch.h"
#include "Bullet.h"
#include "Game.h"
#include "Bug.h"
#include "Tank.h"

IMPLEMENT_RTTI(Bullet);

void Bullet::OnStart(Point)
{
}

void Bullet::OnUpdate(float dt)
{
	const auto tile = g_Game->grid.GetTile(position);
	auto neighbors = g_Game->grid.GetNeighboringTiles(tile, 1);
	neighbors.push_back(tile);
	for (const auto& t : neighbors)
	{
		for (auto& object : g_Game->grid.GetObjsInTile(t))
		{
			if (!object->disabled)
			{
				if (object->position.Distance(position) < object->GetRadius())
				{
					g_Game->tank->score++;
					object->disabled = true;
					object->visible = false;
					g_Game->grid.DeleteObject(object, g_Game->grid.GetTile(object->position));
					disabled = true;
					visible = false;
					return;
				}
			}
		}
	}
}

void Bullet::OnLifeEnd()
{
	disabled = true;
	visible = false;
}
