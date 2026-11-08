#include "pch.h"
#include "RectBar.h"
#include "MyPlayer.h"
#include "SceneMgr.h"

void RectBar::Render(HDC hdc)
{
	if (_sprite == nullptr)
		return;

	Vec2Int size = _sprite->GetSize();

	BLENDFUNCTION bf = {};                    // 전부 0으로 초기화
	bf.BlendOp = AC_SRC_OVER;                 // 합성 방식: 원본을 대상 위에 올린다 (이 값만 허용됨)
	bf.BlendFlags = 0;                        // 예약된 값, 항상 0
	bf.SourceConstantAlpha = 255;          // 이미지 전체에 곱하는 투명도 (0~255)
	bf.AlphaFormat = AC_SRC_ALPHA;            // 이미지 자체의 픽셀별 알파 채널을 사용한다

	::SetStretchBltMode(hdc, COLORONCOLOR);	  // 이미지를 축소하거나 확대해서 그릴 때 픽셀을 어떻게 처리할지 정하는 설정입니다. COLORONCOLOR는 그 방식 중 하나입니다.

	::AlphaBlend(hdc,
		(int32)_pos.x - size.x / 2,
		(int32)_pos.y - size.y / 2,
		size.x,
		size.y,
		_sprite->GetDC(),
		_sprite->GetPos().x,
		_sprite->GetPos().y,
		_sprite->GetSize().x,
		_sprite->GetSize().y,
		bf);

	if (!_uiName.empty() && GET(SceneMgr)->GetMyPlayer() != nullptr) {
		if (_uiName == "HpBar") {
			//Player 에너지바 그리기
			Protocol::ObjectInfo info = GET(SceneMgr)->GetMyPlayer()->info;
			float hpRt = ((float)info.hp() / (float)info.maxhp()) * 100;
			SetWidth(hpRt);
			Utils::DrawRectColored(hdc, Vec2(static_cast<float>(_pos.x + 8), static_cast<float>(_pos.y)), _width, 13, _color, false);
		}
		else if (_uiName == "MpBar") {
			//Player 에너지바 그리기
			Protocol::ObjectInfo info = GET(SceneMgr)->GetMyPlayer()->info;
			float mpRt = ((float)info.hp() / (float)info.maxhp()) * 100;
			SetWidth(mpRt);
			Utils::DrawRectColored(hdc, Vec2(static_cast<float>(_pos.x + 8), static_cast<float>(_pos.y)), _width, 13, _color, false);
		}
		else if (_uiName == "MnBar") {
			Protocol::ObjectInfo info = GET(SceneMgr)->GetMyPlayer()->info;
			int32 money = info.money();
			Utils::DrawTextW(hdc, { 32*5 - 5, 32*2 + 22 }, std::format(L"{}", money), true, 15, RGB(255, 255, 255), true);
		}
	}	
}
