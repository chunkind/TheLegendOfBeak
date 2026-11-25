#pragma once

#include "Panel.h"

class Inven : public Panel
{
	using Super = Panel;
public:
	Inven();
	virtual ~Inven() override;

	virtual void Tick() override;
	virtual void Render(HDC hdc) override;

protected:
	bool _isClick = false;
	POINT _clickPoint = {};
	POINT _movePoint = {};
};
