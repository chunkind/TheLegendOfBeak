#include "pch.h"
#include "StoreNcp.h"
#include "FlipbookActor.h"
#include "Flipbook.h"
#include "Texture.h"
#include "SceneMgr.h"
#include "ResMgr.h"
#include "TimeMgr.h"

namespace
{
	const float PATROL_SPEED = 40.f;	// 초당 이동 픽셀
}

StoreNcp::StoreNcp()
{
	_flipbookIdle[DIR_UP] = GET(ResMgr)->GetFlipbook(L"FB_StoreNcpIdleUp");
	_flipbookIdle[DIR_DOWN] = GET(ResMgr)->GetFlipbook(L"FB_StoreNcpIdleDown");
	_flipbookIdle[DIR_LEFT] = GET(ResMgr)->GetFlipbook(L"FB_StoreNcpIdleLeft");
	_flipbookIdle[DIR_RIGHT] = GET(ResMgr)->GetFlipbook(L"FB_StoreNcpIdleRight");

	_flipbookMove[DIR_UP] = GET(ResMgr)->GetFlipbook(L"FB_StoreNcpMoveUp");
	_flipbookMove[DIR_DOWN] = GET(ResMgr)->GetFlipbook(L"FB_StoreNcpMoveDown");
	_flipbookMove[DIR_LEFT] = GET(ResMgr)->GetFlipbook(L"FB_StoreNcpMoveLeft");
	_flipbookMove[DIR_RIGHT] = GET(ResMgr)->GetFlipbook(L"FB_StoreNcpMoveRight");
}

StoreNcp::~StoreNcp()
{
}

void StoreNcp::BeginPlay()
{
	Super::BeginPlay();

	SetState(MOVE);
	SetState(IDLE);
}

void StoreNcp::Tick()
{
	Super::Tick();


}

void StoreNcp::SetPatrol(const vector<Vec2Int>& waypoints)
{
	_waypoints = waypoints;
	if (_waypoints.empty())
		return;

	SetCellPos(_waypoints[0], true);
	_waypointIdx = (_waypoints.size() > 1) ? 1 : 0;
	_waitSeconds = 0.f;
}

void StoreNcp::TickIdle()
{
	if (_waypoints.empty())
	{

	}
	else
	{
		if (_waitSeconds > 0.f)
		{
			_waitSeconds = max(0.f, _waitSeconds - DT);
			return;
		}

		Vec2Int cell = GetCellPos();
		Vec2Int target = _waypoints[_waypointIdx];
		if (cell == target)
		{
			_waypointIdx = (_waypointIdx + 1) % (int32)_waypoints.size();
			target = _waypoints[_waypointIdx];
			if (cell == target)
				return;
		}

		// 목적지 방향으로 한 칸. 남은 거리가 먼 축을 먼저 시도하고, 막히면 다른 축을 시도한다.
		Vec2Int delta = target - cell;
		vector<Vec2Int> steps;
		if (delta.x != 0)
			steps.push_back(Vec2Int{ delta.x > 0 ? 1 : -1, 0 });
		if (delta.y != 0)
			steps.push_back(Vec2Int{ 0, delta.y > 0 ? 1 : -1 });
		if (steps.size() == 2 && abs(delta.y) > abs(delta.x))
			std::swap(steps[0], steps[1]);

		for (Vec2Int& step : steps)
		{
			Vec2Int nextCell = cell + step;
			if (CanGo(nextCell) == false)
				continue;

			SetDir(GetLookAtDir(nextCell));
			SetCellPos(nextCell);
			SetState(MOVE);
			return;
		}

		// 막혔으면 잠시 후 다시 시도
		_waitSeconds = 0.5f;
	}
}

void StoreNcp::TickMove()
{
	Vec2 dir = (_destPos - _pos);
	if (dir.Length() < 5.f)
	{
		_pos = _destPos;
		SetState(IDLE);

		// 웨이포인트에 도착했으면 다음 목적지를 정하고 잠시 대기
		if (_waypoints.empty() == false && GetCellPos() == _waypoints[_waypointIdx])
		{
			_waypointIdx = (_waypointIdx + 1) % (int32)_waypoints.size();
			_waitSeconds = 1.5f + (rand() % 150) / 100.f;
		}
		return;
	}

	switch (info.dir())
	{
	case DIR_UP:
		_pos.y -= PATROL_SPEED * DT;
		break;
	case DIR_DOWN:
		_pos.y += PATROL_SPEED * DT;
		break;
	case DIR_LEFT:
		_pos.x -= PATROL_SPEED * DT;
		break;
	case DIR_RIGHT:
		_pos.x += PATROL_SPEED * DT;
		break;
	}
}

void StoreNcp::Render(HDC hdc)
{
	//Super::Render(hdc);

	if (_flipbook == nullptr)
		return;

	const FlipbookInfo& info = _flipbook->GetInfo();
	Vec2 cameraPos = GET(SceneMgr)->GetCameraPos();

	BLENDFUNCTION bf = {};                    // 전부 0으로 초기화
	bf.BlendOp = AC_SRC_OVER;                 // 합성 방식: 원본을 대상 위에 올린다 (이 값만 허용됨)
	bf.BlendFlags = 0;                        // 예약된 값, 항상 0
	bf.SourceConstantAlpha = _alpha;          // 이미지 전체에 곱하는 투명도 (0~255)
	bf.AlphaFormat = AC_SRC_ALPHA;            // 이미지 자체의 픽셀별 알파 채널을 사용한다

	::SetStretchBltMode(hdc, COLORONCOLOR);	  // 이미지를 축소하거나 확대해서 그릴 때 픽셀을 어떻게 처리할지 정하는 설정입니다. COLORONCOLOR는 그 방식 중 하나입니다.

	::AlphaBlend(hdc,
		(int32)_pos.x - info.size.x / 2 - ((int32)cameraPos.x - GWinSizeX / 2),
		(int32)_pos.y - info.size.y + GPixcelHeight / 2 - ((int32)cameraPos.y - GWinSizeY / 2),
		info.size.x,
		info.size.y,
		info.texture->GetDC(),
		(info.start + _idx) * info.size.x,
		info.line * info.size.y,
		info.size.x,
		info.size.y,
		bf);
}

void StoreNcp::UpdateAnimation()
{
	switch (info.state())
	{
	case IDLE:
		SetFlipbook(_flipbookIdle[info.dir()]);
		break;
	case MOVE:
		SetFlipbook(_flipbookMove[info.dir()]);
		break;
	}
}
