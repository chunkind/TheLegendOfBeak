#pragma once

#include "FlipbookActor.h"
#include "Creature.h"

class Flipbook;

class StoreNcp : public Creature
{
	using Super = Creature;

public:
	StoreNcp();
	virtual ~StoreNcp() override;

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc) override;

	virtual void TickIdle() override;
	virtual void TickMove() override;

	virtual void UpdateAnimation() override;

	// 순환할 웨이포인트(셀 좌표) 설정. 첫 웨이포인트에서 시작한다.
	void SetPatrol(const vector<Vec2Int>& waypoints);

protected:
	Flipbook* _flipbookIdle[4] = {};
	Flipbook* _flipbookMove[4] = {};

	//Flipbook* _flipbook = nullptr;

	uint8 _alpha = 1000;

	vector<Vec2Int> _waypoints;
	int32 _waypointIdx = 0;		// 현재 향하는 웨이포인트
	float _waitSeconds = 0.f;	// 남은 대기 시간
};
