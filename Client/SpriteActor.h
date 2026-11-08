#pragma once
#include "Actor.h"

class Sprite;

class SpriteActor : public Actor
{
	using Super = Actor;
public:
	SpriteActor();
	virtual ~SpriteActor() override;

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc) override;

	void SetSprite(Sprite* sprite) { _sprite = sprite; }
	Sprite* GetSprite() { return _sprite; }
	void SetCenterPos(bool isCenter) { _isCenterPos = isCenter; }

	void SetAlpha(bool alpha) { _isAlpha = alpha; }
	void SetFooter(bool footer) { _footer = footer; }

	void RenderAlpha(HDC hdc, Vec2Int size, int xPos, int yPos);
	void RenderNonAlpha(HDC hdc, Vec2Int size, int xPos, int yPos);

protected:
	Sprite* _sprite = nullptr;
	bool _isCenterPos = true;
	bool _isAlpha = false;
	bool _footer = false;
};

