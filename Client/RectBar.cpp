#include "pch.h"
#include "RectBar.h"
#include "MyPlayer.h"
#include "SceneMgr.h"

void RectBar::Render(HDC hdc)
{
	if (_sprite == nullptr)
		return;

	Vec2Int size = _sprite->GetSize();
	
	::TransparentBlt(hdc,
		(int32)_pos.x - size.x / 2,
		(int32)_pos.y - size.y / 2,
		size.x,
		size.y,
		_sprite->GetDC(),
		_sprite->GetPos().x,
		_sprite->GetPos().y,
		_sprite->GetSize().x,
		_sprite->GetSize().y,
		RGB(255, 0, 255));

	if (!_uiName.empty() && GET(SceneMgr)->GetMyPlayer() != nullptr) {
		if (_uiName == "HpBar") {
			//Player 에너지바 그리기
			Protocol::ObjectInfo info = GET(SceneMgr)->GetMyPlayer()->info;
			float hpRt = ((float)info.hp() / (float)info.maxhp()) * 100;
			SetWidth(hpRt);
		}
		else if (_uiName == "MpBar") {
			//Player 에너지바 그리기
			Protocol::ObjectInfo info = GET(SceneMgr)->GetMyPlayer()->info;
			float mpRt = ((float)info.hp() / (float)info.maxhp()) * 100;
			SetWidth(mpRt);
		}
	}
	
	Utils::DrawRectColored(hdc, Vec2(static_cast<float>(_pos.x + 8), static_cast<float>(_pos.y)), _width, 13, _color, false);
}
