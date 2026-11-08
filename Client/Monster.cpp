#include "pch.h"
#include "Monster.h"
#include "InputMgr.h"
#include "TimeMgr.h"
#include "ResMgr.h"
#include "Flipbook.h"
#include "CameraComponent.h"
#include "SceneMgr.h"
#include "GameScene.h"
#include "Player.h"
#include "HitEffect.h"

Monster::Monster()
{
	_flipbookMove[DIR_UP] = GET(ResMgr)->GetFlipbook(L"FB_SnakeUp");
	_flipbookMove[DIR_DOWN] = GET(ResMgr)->GetFlipbook(L"FB_SnakeDown");
	_flipbookMove[DIR_LEFT] = GET(ResMgr)->GetFlipbook(L"FB_SnakeLeft");
	_flipbookMove[DIR_RIGHT] = GET(ResMgr)->GetFlipbook(L"FB_SnakeRight");

}

Monster::~Monster()
{

}

void Monster::BeginPlay()
{
	Super::BeginPlay();

	SetState(MOVE);
	SetState(IDLE);
}

void Monster::Tick()
{
	Super::Tick();

}

void Monster::Render(HDC hdc)
{
	Super::Render(hdc);

	Vec2 cameraPos = GET(SceneMgr)->GetCameraPos();
	string name = info.name();
	float hp = info.hp();
	float maxHp = info.maxhp();
	float hpRt = (hp / maxHp) * 100;
	Utils::DrawTextW(hdc
		, Vec2(static_cast<float>(_pos.x - 35 - ((int32)cameraPos.x - GWinSizeX / 2)), static_cast<float>(_pos.y - 90 - ((int32)cameraPos.y - GWinSizeY / 2)))
		, CoreUtil::TrnWstr(name));
	Utils::DrawRectColored(hdc
		, Vec2(static_cast<float>(_pos.x - ((int32)cameraPos.x - GWinSizeX / 2)), static_cast<float>(_pos.y - 60 - ((int32)cameraPos.y - GWinSizeY / 2)))
		, hpRt
		, 13
		, RGB(255, 0, 0), false);
}

void Monster::TickIdle()
{
	GameScene* scene = dynamic_cast<GameScene*>(GET(SceneMgr)->GetCurrentScene());
	if (scene == nullptr)
		return;

	return;

	// Find Player
	if (_target == nullptr)
		_target = scene->FindClosestPlayer(GetCellPos());

	if (_target)
	{
		Vec2Int dir = _target->GetCellPos() - GetCellPos();
		int32 dist = abs(dir.x) + abs(dir.y);
		if (dist == 1)
		{
			SetDir(GetLookAtDir(_target->GetCellPos()));
			SetState(SKILL);
			_waitSeconds = 0.5f;
		}
		else
		{
			vector<Vec2Int> path;
			if (scene->FindPath(GetCellPos(), _target->GetCellPos(), OUT path))
			{
				if (path.size() > 1)
				{
					Vec2Int nextPos = path[1];
					if (scene->CanGo(nextPos))
					{
						SetCellPos(nextPos);
						SetState(MOVE);
					}
				}
				else
					SetCellPos(path[0]);
			}
		}
	}
}

void Monster::TickMove()
{
	Vec2 dir = (_destPos - _pos);
	if (dir.Length() < 5.f)
	{
		SetState(IDLE);
		_pos = _destPos;
	}
	else
	{
		bool horizontal = abs(dir.x) > abs(dir.y);
		if (horizontal)
			SetDir(dir.x < 0 ? DIR_LEFT : DIR_RIGHT);
		else
			SetDir(dir.y < 0 ? DIR_UP : DIR_DOWN);

		switch (info.dir())
		{
		case DIR_UP:
			_pos.y -= 50 * DT;
			break;
		case DIR_DOWN:
			_pos.y += 50 * DT;
			break;
		case DIR_LEFT:
			_pos.x -= 50 * DT;
			break;
		case DIR_RIGHT:
			_pos.x += 50 * DT;
			break;
		}
	}
}

void Monster::TickSkill()
{
	if (_flipbook == nullptr)
		return;

	if (_waitSeconds > 0)
	{
		_waitSeconds = max(0, _waitSeconds - DT);
		return;
	}

	{
		GameScene* scene = dynamic_cast<GameScene*>(GET(SceneMgr)->GetCurrentScene());
		if (scene == nullptr)
			return;

		Creature* creature = scene->GetCreatureAt(GetFrontCellPos());
		if (creature)
		{
			scene->SpawnObject<HitEffect>(GetFrontCellPos());
			creature->OnDamaged(this);
		}

		SetState(IDLE);
	}
}

void Monster::UpdateAnimation()
{
	SetFlipbook(_flipbookMove[info.dir()]);
}
