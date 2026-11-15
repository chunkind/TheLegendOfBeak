#pragma once

#include "GameObject.h"

class Sprite;

class Event : public GameObject
{
	using Super = GameObject;
public:
	Event();
	virtual ~Event() override;

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc) override;

	void SetSprite(Sprite* sprite) { _sprite = sprite; }
	void StartEvent() { _isStart = true; }

	void SetSize(Vec2Int size) { _size = size; }
	Vec2Int GetSize() { return _size; }

protected:
	Sprite* _sprite = nullptr;
	bool _isStart = false;
	bool _isRender = false;
	Vec2Int _size = { 100, 100 };
};
