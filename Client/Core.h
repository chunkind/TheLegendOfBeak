#pragma once

class Core
{
	SING(Core)

public:
	void Init(HWND hwnd);
	void Update();
	void Render();

	int32 GetScreenWidth() { return _rect.right - _rect.left; }
	int32 GetScreenHeight() { return _rect.bottom - _rect.top; }

	HDC GetHdc() { return hdc; }
	HWND GetHwnd() { return _hwnd; }

private:
	HWND _hwnd = {};
	HDC hdc = {};

private:
	// Double Buffering
	RECT _rect;
	HDC hdcBack = {};
	HBITMAP _bmpBack = {};
};

