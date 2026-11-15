#pragma once
#include "FlipbookActor.h"

class GameObject : public FlipbookActor
{
	using Super = FlipbookActor;

public:
	GameObject();
	virtual ~GameObject() override;

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc);

	virtual void TickIdle() {}
	virtual void TickMove() {}
	virtual void TickSkill() {}
	virtual void TickDead() {}
	virtual void TickEvent() {}

	void SetState(ObjectState state);
	void SetDir(Dir dir);

	virtual void UpdateAnimation() {}

	bool HasReachedDest();
	bool CanGo(Vec2Int cellPos);
	Dir GetLookAtDir(Vec2Int cellPos);

	void SetCellPos(Vec2Int cellPos, bool teleport = false);
	Vec2Int GetCellPos();
	Vec2Int GetFrontCellPos();
	Vec2Int GetLineCellPos();

	int64 GetObjectID() { return info.objectid(); }
	void SetObjectID(int64 id) { info.set_objectid(id); }

protected:
	bool _dirtyFlag = false;

public:
	Protocol::ObjectInfo info;
};

