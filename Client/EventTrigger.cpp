#include "pch.h"
#include "EventTrigger.h"
#include "Sprite.h"
#include "Utils.h"
#include "InputMgr.h"

EventTrigger::EventTrigger()
{
	_layer = LAYER_EVENT;
}

EventTrigger::~EventTrigger()
{}

void EventTrigger::BeginPlay()
{
}

void EventTrigger::Tick()
{
	if (_isStart && _isRender)
	{
		if (GET(InputMgr)->GetButtonDown(KeyType::SpaceBar))
		{
			_isStart = false;
		}
	}
	
}

void EventTrigger::Render(HDC hdc)
{
	if (_isStart)
	{
		_isRender = true;

		if (_sprite == nullptr)
			return;

		// 32bit BMP(premultiplied alpha)의 알파 채널로 투명 처리 + 전체 반투명
		BLENDFUNCTION bf = {};                    // 전부 0으로 초기화
		bf.BlendOp = AC_SRC_OVER;                 // 합성 방식: 원본을 대상 위에 올린다 (이 값만 허용됨)
		bf.BlendFlags = 0;                        // 예약된 값, 항상 0
		bf.SourceConstantAlpha = 255;          // 이미지 전체에 곱하는 투명도 (0~255)
		bf.AlphaFormat = AC_SRC_ALPHA;            // 이미지 자체의 픽셀별 알파 채널을 사용한다

		::SetStretchBltMode(hdc, COLORONCOLOR);

		::AlphaBlend(hdc,
			32 * 0, 32 * 13,
			32 * 25, 32 * 6,
			_sprite->GetDC(),
			_sprite->GetPos().x,
			_sprite->GetPos().y,
			_sprite->GetSize().x,
			_sprite->GetSize().y,
			bf);

		Utils::DrawTextW(hdc, {32 * 1, 32 * 14}, L"안녕하세요. 상점입니다.", true, 20);

	}
	else
	{
		_isRender = false;
	}
}
