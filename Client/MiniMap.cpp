#include "pch.h"
#include "MiniMap.h"

MiniMap::MiniMap()
{
}

MiniMap::~MiniMap()
{
}

void MiniMap::Render(HDC hdc)
{
	if (_sprite == nullptr)
		return;

	// 32bit BMP(premultiplied alpha)의 알파 채널로 투명 처리 + 전체 반투명
	BLENDFUNCTION bf = {};
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.SourceConstantAlpha = _alpha;
	bf.AlphaFormat = AC_SRC_ALPHA;

	::SetStretchBltMode(hdc, COLORONCOLOR);

	::AlphaBlend(hdc,
		(int32)_pos.x - _size.x / 2,
		(int32)_pos.y - _size.y / 2,
		_size.x,
		_size.y,
		_sprite->GetDC(),
		_sprite->GetPos().x,
		_sprite->GetPos().y,
		_sprite->GetSize().x,
		_sprite->GetSize().y,
		bf);

	// 점찍기
	PrintDot(hdc, _size.x, _size.y, 8, RGB(255, 60, 60));
	
}

void MiniMap::PrintDot(HDC hdc, int32 x, int32 y, int32 size, COLORREF color)
{
	Utils::DrawDot(hdc, 
		{ _pos.x - _size.x/2 + x, _pos.y - _size.y/2 + y },
		size, 
		color);
	//Utils::DrawDot(hdc, {}, 2, RGB(80, 160, 255));
}
