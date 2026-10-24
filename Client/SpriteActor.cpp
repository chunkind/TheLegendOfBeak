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

	::TransparentBlt(hdc,
		startXpos,
		startYpos,
		size.x,
		size.y,
		_sprite->GetDC(),
		_sprite->GetPos().x,
		_sprite->GetPos().y,
		_sprite->GetSize().x,
		_sprite->GetSize().y,
		_sprite->GetTransparent());

	/*::BitBlt(hdc,
		startXpos,
		startYpos,
		size.x,
		size.y,
		_sprite->GetDC(),
		_sprite->GetPos().x,
		_sprite->GetPos().y,
		SRCCOPY);*/
}
