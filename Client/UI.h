#pragma once


class UI
{
public:
	UI();
	virtual ~UI();

	virtual void BeginPlay();
	virtual void Tick();
	virtual void Render(HDC hdc);

	void SetPos(Vec2 pos) { _pos = pos; }
	Vec2 GetPos() { return _pos; }

	void SetSize(Vec2Int size) { _size = size; }
	Vec2Int GetSize() { return _size; }

	void SetUIName(string uiName) { _uiName = uiName; }
	string GetUIName() { return _uiName; }

	void SetVisible(bool visible) { _visible = visible; }
	bool IsVisible() { return _visible; }

	RECT GetRect();
	bool IsMouseInRect();

protected:
	bool	_visible = true;
	string	_uiName = "";
	Vec2	_pos = { 400, 300 };
	Vec2Int _size = { 150, 150 };

};

