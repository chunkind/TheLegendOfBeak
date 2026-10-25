#pragma once

#include "UI.h"
#include "Sprite.h"

class Tilemap;

class MiniMap : public UI
{
public:
	MiniMap();
	virtual ~MiniMap() override;

	virtual void BeginPlay() override;
	virtual void Render(HDC hdc) override;

	void SetSprite(Sprite* sprite) { _sprite = sprite; }
	void SetAlpha(uint8 alpha) { _alpha = alpha; }

	void PrintDot(HDC hdc, int32 x, int32 y, int32 size, COLORREF color);

protected:
	uint8		_alpha			= 200; // 이미지 알파에 곱해지는 전체 투명도 (0: 완전 투명, 255: 이미지 그대로)
	float		_width			= 100;

	Sprite*		_sprite			= nullptr;
	Tilemap*	_tm				= nullptr;
	
	Vec2Int		_scrinSize		= {_size.x - 30, _size.y - 30};

	HDC			_terrainDC		= nullptr; // 지형을 미리 그려둘 메모리 DC
	HBITMAP		_terrainBitmap	= nullptr; // 메모리 DC에 연결된 비트맵 (맵 셀 수 크기)
	HBITMAP		_prevBitmap		= nullptr; // 해제할 때 되돌릴 원래 비트맵
};

