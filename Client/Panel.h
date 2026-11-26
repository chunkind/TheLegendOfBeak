#pragma once
#include "UI.h"

class Sprite;
class TopPanel;

class Panel : public UI
{
	using Super = UI;
public:
	Panel();
	virtual ~Panel();

	virtual void BeginPlay();
	virtual void Tick();
	virtual void Render(HDC hdc);

	void		AddHeader(TopPanel* header) { _header = header; }
	void		AddChild(UI* ui);
	bool		RemoveChild(UI* ui);

private:
	TopPanel*		_header		= nullptr;
	vector<UI*>		_children;
};

