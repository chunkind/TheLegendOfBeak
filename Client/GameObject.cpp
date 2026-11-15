#include "pch.h"
#include "GameObject.h"
#include "Creature.h"
#include "InputMgr.h"
#include "TimeMgr.h"
#include "ResMgr.h"
#include "Flipbook.h"
#include "CameraComponent.h"
#include "SceneMgr.h"
#include "GameScene.h"

GameObject::GameObject()
{

}

GameObject::~GameObject()
{

}

void GameObject::BeginPlay()
{
	Super::BeginPlay();

	SetState(MOVE);
	SetState(IDLE);
}

void GameObject::Tick()
{
	_dirtyFlag = false;

	switch (info.state())
	{
	case IDLE:
		TickIdle();
		break;
	case MOVE:
		TickMove();
		break;
	case SKILL:
		TickSkill();
		break;
	case EVENT:
		TickEvent();
		break;
	case DEAD:
		TickDead();
		break;
	}

	Super::Tick();
}

void GameObject::Render(HDC hdc)
{
	Super::Render(hdc);
}

void GameObject::SetState(ObjectState state)
{
	if (info.state() == state)
		return;

	info.set_state(state);
	UpdateAnimation();
	_dirtyFlag = true;
}

void GameObject::SetDir(Dir dir)
{
	info.set_dir(dir);
	UpdateAnimation();
	_dirtyFlag = true;
}

bool GameObject::HasReachedDest()
{
	Vec2 dir = (_destPos - _pos);
	return (dir.Length() < 5.f);
}

bool GameObject::CanGo(Vec2Int cellPos)
{
	GameScene* scene = dynamic_cast<GameScene*>(GET(SceneMgr)->GetCurrentScene());
	if (scene == nullptr)
		return false;

	return scene->CanGo(cellPos);
}

Dir GameObject::GetLookAtDir(Vec2Int cellPos)
{
	Vec2Int dir = cellPos - GetCellPos();
	if (dir.x > 0)
		return DIR_RIGHT;
	else if (dir.x < 0)
		return DIR_LEFT;
	else if (dir.y > 0)
		return DIR_DOWN;
	else
		return DIR_UP;
}

void GameObject::SetCellPos(Vec2Int cellPos, bool teleport /*= false*/)
{
	info.set_posx(cellPos.x);
	info.set_posy(cellPos.y);

	GameScene* scene = dynamic_cast<GameScene*>(GET(SceneMgr)->GetCurrentScene());
	if (scene == nullptr)
		return;

	_destPos = scene->ConvertPos(cellPos);

	if (teleport)
		_pos = _destPos;

	_dirtyFlag = true;
}

Vec2Int GameObject::GetCellPos()
{
	return Vec2Int{ info.posx(), info.posy() };
}

Vec2Int GameObject::GetFrontCellPos()
{
	switch (info.dir())
	{
	case DIR_DOWN:
		return GetCellPos() + Vec2Int{ 0, 1 };
	case DIR_LEFT:
		return GetCellPos() + Vec2Int{ -1, 0 };
	case DIR_RIGHT:
		return GetCellPos() + Vec2Int{ 1, 0 };
	case DIR_UP:
		return GetCellPos() + Vec2Int{ 0, -1 };
	}

	return GetCellPos();
}

Vec2Int GameObject::GetLineCellPos()
{
	GameScene* scene = dynamic_cast<GameScene*>(GET(SceneMgr)->GetCurrentScene());
	if (scene == nullptr)
		return Vec2Int(-1, -1);

	vector<Actor*> list = scene->_actors[LAYER_OBJECT];
	for (Actor* actor : list)
	{
		Creature* creature = dynamic_cast<Creature*>(actor);
		if (creature == nullptr)
			continue;

		string creatureName = creature->info.name();
		int32 creatureX = creature->info.posx();
		int32 creatureY = creature->info.posy();
		int32 playerX = info.posx();
		int32 playerY = info.posy();

		if (creatureX == playerX && creatureY == playerY)
			continue;

		switch (info.dir())
		{
		case DIR_DOWN:
			if (playerX == creatureX && playerY < creatureY)
				return creature->GetCellPos();
			break;
		case DIR_UP:
			if (playerX == creatureX && playerY > creatureY)
				return creature->GetCellPos();
			break;
		case DIR_LEFT:
			if (playerY == creatureY && playerX > creatureX)
				return creature->GetCellPos();
			break;
		case DIR_RIGHT:
			if (playerY == creatureY && playerX < creatureX)
				return creature->GetCellPos();
			break;
		}
	}
	return Vec2Int(-1, -1);
}
