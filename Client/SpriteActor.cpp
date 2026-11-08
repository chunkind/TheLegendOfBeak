#include "pch.h"
#include "SpriteActor.h"
#include "Sprite.h"
#include "SceneMgr.h"

SpriteActor::SpriteActor()
{

}

SpriteActor::~SpriteActor()
{

}

void SpriteActor::BeginPlay()
{
	Super::BeginPlay();


}

void SpriteActor::Tick()
{
	Super::Tick();


}

void SpriteActor::Render(HDC hdc)
{
	Super::Render(hdc);

	if (_sprite == nullptr)
		return;

	Vec2Int size = _sprite->GetSize();
	Vec2 cameraPos = GET(SceneMgr)->GetCameraPos();


	int32 startXpos = 0;
	int32 startYpos = 0;
	if (_isCenterPos)
	{
		startXpos = (int32)_pos.x - size.x / 2 - ((int32)cameraPos.x - GWinSizeX / 2);
		startYpos = (int32)_pos.y - size.y / 2 - ((int32)cameraPos.y - GWinSizeY / 2);
	}
	else
	{
		startXpos = (int32)_pos.x - ((int32)cameraPos.x - GWinSizeX / 2);
		startYpos = (int32)_pos.y - ((int32)cameraPos.y - GWinSizeY / 2);
	}

	if (_isAlpha)
		RenderAlpha(hdc, size, startXpos, startYpos);
	else
		RenderNonAlpha(hdc, size, startXpos, startYpos);
}

void SpriteActor::RenderAlpha(HDC hdc, Vec2Int size, int xPos, int yPos)
{
	BLENDFUNCTION bf = {};                    // 전부 0으로 초기화
	bf.BlendOp = AC_SRC_OVER;                 // 합성 방식: 원본을 대상 위에 올린다 (이 값만 허용됨)
	bf.BlendFlags = 0;                        // 예약된 값, 항상 0
	bf.SourceConstantAlpha = 255;          // 이미지 전체에 곱하는 투명도 (0~255)
	bf.AlphaFormat = AC_SRC_ALPHA;            // 이미지 자체의 픽셀별 알파 채널을 사용한다

	::SetStretchBltMode(hdc, COLORONCOLOR);	  // 이미지를 축소하거나 확대해서 그릴 때 픽셀을 어떻게 처리할지 정하는 설정입니다. COLORONCOLOR는 그 방식 중 하나입니다.

	::AlphaBlend(hdc,
		xPos,
		yPos,
		size.x,
		size.y,
		_sprite->GetDC(),
		_sprite->GetPos().x,
		_sprite->GetPos().y,
		_sprite->GetSize().x,
		_sprite->GetSize().y,
		bf);
}

void SpriteActor::RenderNonAlpha(HDC hdc, Vec2Int size, int xPos, int yPos)
{
	::TransparentBlt(hdc,
		xPos,
		yPos,
		size.x,
		size.y,
		_sprite->GetDC(),
		_sprite->GetPos().x,
		_sprite->GetPos().y,
		_sprite->GetSize().x,
		_sprite->GetSize().y,
		_sprite->GetTransparent());

	/*::BitBlt(hdc,
		xPos,
		yPos,
		size.x,
		size.y,
		_sprite->GetDC(),
		_sprite->GetPos().x,
		_sprite->GetPos().y,
		SRCCOPY);*/
}
