#include "pch.h"
#include "Inven.h"
#include "InputMgr.h"

Inven::Inven()
{
	_size = { 300, 360 };
	_pos = { 0, 0 };
	SetVisible(true);
	_isClick = false;
}

Inven::~Inven()
{
}

void Inven::Tick()
{
	if (GET(InputMgr)->GetButtonDown(KeyType::LeftMouse)) {
		if (IsMouseInRect())
		{
			_isClick = true;
			_clickPoint = GET(InputMgr)->GetMousePos();
		}
		
	}

	if (GET(InputMgr)->GetButtonUp(KeyType::LeftMouse)) {
		_isClick = false;
	}

	if (_isClick)
	{
		POINT mousePos = GET(InputMgr)->GetMousePos();
		_movePoint.x = mousePos.x - _clickPoint.x;
		_movePoint.y = mousePos.y - _clickPoint.y;

		_pos.x += _movePoint.x;
		_pos.y += _movePoint.y;

		_clickPoint = mousePos;
	}
}

void Inven::Render(HDC hdc)
{
	Super::Render(hdc);

}
