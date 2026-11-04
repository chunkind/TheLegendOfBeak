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

	virtual void UpdateAnimation() override;

protected:
	Flipbook* _flipbookIdle[4] = {};
	Flipbook* _flipbookMove[4] = {};

	//Flipbook* _flipbook = nullptr;

	uint8 _alpha = 1000;
};
