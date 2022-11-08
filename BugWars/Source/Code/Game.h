#pragma once
#include "GameBase/GameBase.h"
#include "Globals.h"

#include "PartitionGrid.h"

struct Tank;

struct Game : public GameBase
{
	Game();

	virtual void OnUpdate(float dt) override;
	virtual void OnRender() const override;
	virtual void AddObject(GameObject* object) override;

	virtual void OnBugsSpawned() override;

	std::vector<GameObject*> objects;

	PartitionGrid grid{};

	~Game();
};