#include "pch.h"
#include "StoreNcp.h"
#include "FlipbookActor.h"
#include "Flipbook.h"
#include "Texture.h"
#include "SceneMgr.h"
#include "ResMgr.h"

StoreNcp::StoreNcp()
{
	_flipbookIdle[DIR_UP] = GET(ResMgr)->GetFlipbook(L"FB_StoreNcpIdleUp");
	_flipbookIdle[DIR_DOWN] = GET(ResMgr)->GetFlipbook(L"FB_StoreNcpIdleDown");
	_flipbookIdle[DIR_LEFT] = GET(ResMgr)->GetFlipbook(L"FB_StoreNcpIdleLeft");
	_flipbookIdle[DIR_RIGHT] = GET(ResMgr)->GetFlipbook(L"FB_StoreNcpIdleRight");

	_flipbookMove[DIR_UP] = GET(ResMgr)->GetFlipbook(L"FB_StoreNcpMoveUp");
	_flipbookMove[DIR_DOWN] = GET(ResMgr)->GetFlipbook(L"FB_StoreNcpMoveDown");
	_flipbookMove[DIR_LEFT] = GET(ResMgr)->GetFlipbook(L"FB_StoreNcpMoveLeft");
	_flipbookMove[DIR_RIGHT] = GET(ResMgr)->GetFlipbook(L"FB_StoreNcpMoveRight");
}

StoreNcp::~StoreNcp()
{
}

void StoreNcp::BeginPlay()
{
	Super::BeginPlay();

	SetState(MOVE);
	SetState(IDLE);
}

void StoreNcp::Tick()
{
	Super::Tick();


}

void StoreNcp::Render(HDC hdc)
{
	//Super::Render(hdc);

	if (_flipbook == nullptr)
		return;

	const FlipbookInfo& info = _flipbook->GetInfo();
	Vec2 cameraPos = GET(SceneMgr)->GetCameraPos();

	BLENDFUNCTION bf = {};                    // 전부 0으로 초기화
	bf.BlendOp = AC_SRC_OVER;                 // 합성 방식: 원본을 대상 위에 올린다 (이 값만 허용됨)
	bf.BlendFlags = 0;                        // 예약된 값, 항상 0
	bf.SourceConstantAlpha = _alpha;          // 이미지 전체에 곱하는 투명도 (0~255)
	bf.AlphaFormat = AC_SRC_ALPHA;            // 이미지 자체의 픽셀별 알파 채널을 사용한다

	::SetStretchBltMode(hdc, COLORONCOLOR);	  // 이미지를 축소하거나 확대해서 그릴 때 픽셀을 어떻게 처리할지 정하는 설정입니다. COLORONCOLOR는 그 방식 중 하나입니다.

	::AlphaBlend(hdc,
		(int32)_pos.x - info.size.x / 2 - ((int32)cameraPos.x - GWinSizeX / 2),
		(int32)_pos.y - info.size.y + GPixcelHeight / 2 - ((int32)cameraPos.y - GWinSizeY / 2),
		info.size.x,
		info.size.y,
		info.texture->GetDC(),
		(info.start + _idx) * info.size.x,
		info.line * info.size.y,
		info.size.x,
		info.size.y,
		bf);
}

void StoreNcp::UpdateAnimation()
{
	switch (info.state())
	{
	case IDLE:
		SetFlipbook(_flipbookIdle[info.dir()]);
		break;
	case MOVE:
		SetFlipbook(_flipbookMove[info.dir()]);
		break;
	}
}
