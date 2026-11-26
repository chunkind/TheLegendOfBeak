#pragma once

#include "UI.h"

class TopPanel : public UI
{
	using Super = UI;
public:
	TopPanel();
	virtual ~TopPanel();

	virtual void BeginPlay();
	virtual void Tick();
	virtual void Render(HDC hdc);

	
};

