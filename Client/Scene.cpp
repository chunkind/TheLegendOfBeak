#include "pch.h"
#include "Scene.h"
#include "Actor.h"
#include "Creature.h"
#include "UI.h"
#include "TimeMgr.h"
#include "SceneMgr.h"

Scene::Scene()
{

}

Scene::~Scene()
{
	for (const vector<Actor*>& actors : _actors)
		for (Actor* actor : actors)
			SAFE_DELETE(actor);

	_actors->clear();

	for (UI* ui : _uis)
		SAFE_DELETE(ui);

	_uis.clear();
}

void Scene::Init()
{
	for (const vector<Actor*>& actors : _actors)
		for (Actor* actor : actors)
			actor->BeginPlay();

	for (UI* ui : _uis)
		ui->BeginPlay();
}

void Scene::Update()
{
	float deltaTime = DT;

	for (const vector<Actor*> actors : _actors)
		for (Actor* actor : actors)
			actor->Tick();

	for (UI* ui : _uis)
		ui->Tick();
}

void Scene::Render(HDC hdc)
{
	vector<Actor*>& actors = _actors[LAYER_OBJECT];
	std::sort(actors.begin(), actors.end(), [=](Actor* a, Actor* b)
		{
			return a->GetPos().y < b->GetPos().y;
		});

	for (const vector<Actor*>& actors : _actors)
		for (Actor* actor : actors)
			actor->Render(hdc);

	for (UI* ui : _uis)
		ui->Render(hdc);
}

void Scene::AddActor(Actor* actor)
{
	if (actor == nullptr)
		return;

	_actors[actor->GetLayer()].push_back(actor);
}

void Scene::RemoveActor(Actor* actor)
{
	if (actor == nullptr)
		return;

	vector<Actor*>& v = _actors[actor->GetLayer()];
	v.erase(std::remove(v.begin(), v.end(), actor), v.end());
}

void Scene::AddUI(UI* ui)
{
	if (ui == nullptr)
		return;

	_uis.push_back(ui);
}

void Scene::RemoveUI(UI* ui)
{
	if (ui == nullptr)
		return;

	_uis.erase(std::remove(_uis.begin(), _uis.end(), ui), _uis.end());
}

Creature* Scene::GetCreatureAt(Vec2Int cellPos)
{
	if (cellPos.x < 0 || cellPos.y < 0)
		return nullptr;

	for (Actor* actor : _actors[LAYER_OBJECT])
	{
		// GameObjectType
		Creature* creature = dynamic_cast<Creature*>(actor);
		if (creature && creature->GetCellPos() == cellPos)
			return creature;
	}

	return nullptr;
}
