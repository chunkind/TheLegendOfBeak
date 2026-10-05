#pragma once
#include <windows.h>
#include <string>
using namespace std;

class Utils
{
public:
	static void DrawText(HDC hdc, Pos pos, const wstring& str, bool bold = false, int32 fontSize = 20, COLORREF color = RGB(255, 255, 255));

	static void DrawRect(HDC hdc, Pos pos, int32 w, int32 h);

	static void DrawCircle(HDC hdc, Pos pos, int32 radius);

	static void DrawLine(HDC hdc, Pos from, Pos to);

	static void DrawRectColored(HDC hdc, Pos pos, int32 w, int32 h, COLORREF color, bool isAutoPos=true);

	static void DrawLineColored(HDC hdc, Pos from, Pos to, COLORREF color);

	static void DrawRectAlpha(HDC hdc, Pos pos, int32 w, int32 h, COLORREF color, BYTE alpha);

	static void DrawRectBorder(HDC hdc, Pos pos, int32 w, int32 h, COLORREF color, int32 thickness = 1);

	static void ReadBmp(const wstring& path);
};

