#pragma once

#include "UI.h"
#include "Sprite.h"

class MiniMap : public UI
{
public:
	MiniMap();
	virtual ~MiniMap() override;

	virtual void Render(HDC hdc) override;

	void SetSprite(Sprite* sprite) { _sprite = sprite; }
	void SetAlpha(uint8 alpha) { _alpha = alpha; }

	void PrintDot(HDC hdc, int32 x, int32 y, int32 size, COLORREF color);
protected:
	float _width = 100;
	Sprite* _sprite = nullptr;

	uint8 _alpha = 200;			// 이미지 알파에 곱해지는 전체 투명도 (0: 완전 투명, 255: 이미지 그대로)

};

