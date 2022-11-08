#include "pch.h"
#include "Game.h"
#include "GameBase/Log.h"
#include "Tank.h"
#include "Bug.h"
#include "Bullet.h"

#include <numeric>

Game* g_Game;

Game::Game()
	: GameBase({ [] { return new Tank; },
				 [] { return new Bug; },
				 [] { return new Bullet; }})
{
	g_Game = this;
}

// TODO Don't draw objects off screen
void Game::OnUpdate(float dt)
{
	PIXScopedEvent(PIX_COLOR_INDEX(5), __FUNCTION__);
	for (auto obj : objects)
		if (!obj->disabled)
			obj->Update(dt);

	auto start = std::stable_partition(objects.begin(), objects.end(),
		[](auto& obj) { return !obj->disabled; });
	std::for_each(start, objects.end(), [](auto& obj) { delete obj; });
	objects.erase(start, objects.end());
}

void Game::OnRender() const
{
	for (auto obj : objects)
		if (obj->visible)
			DoRender(obj);
}

void Game::AddObject(GameObject* object)
{
	objects.push_back(object);
	if (object->GetRTTI() == Bug::s_RTTI)
	{
		grid.AddObject(object, object->position);
		auto* bug = static_cast<Bug*>(object);
		bug->lastTile = grid.GetTile(bug->position);
	}
}

void Game::OnBugsSpawned()
{
	//auto count = std::accumulate(grid.m_map.begin(), grid.m_map.end(), 0u, [](auto accum, auto pair) { return accum + pair.second.size(); });
}

Game::~Game()
{
	for (auto obj : objects) {
		delete obj;
	}
}