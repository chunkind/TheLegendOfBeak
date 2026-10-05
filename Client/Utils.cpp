#include "pch.h"
#include "Utils.h"

void Utils::DrawText(HDC hdc, Pos pos, const wstring& str, bool bold, int32 fontSize, COLORREF color)
{
	// 1. 폰트 생성 (높이를 음수로 주면 글자 자체의 높이 기준)
	HFONT hFont = ::CreateFont(
		-fontSize,              // cHeight : 크기 조절은 이 값으로
		0,                      // cWidth  : 0이면 자동
		0, 0,                   // 기울기 각도
		bold ? FW_BOLD : FW_NORMAL, // 굵기 (bold 인자로 선택)
		FALSE, FALSE, FALSE,    // 기울임, 밑줄, 취소선
		HANGUL_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		L"Malgun Gothic");

	// 2. DC에 선택하고 기존 폰트 보관
	HFONT hOldFont = (HFONT)::SelectObject(hdc, hFont);

	// 3. 글자 배경 투명 + 글자색 지정
	int32 oldBkMode = ::SetBkMode(hdc, TRANSPARENT);
	COLORREF oldColor = ::SetTextColor(hdc, color);

	// 4. 출력
	::TextOut(hdc
		, static_cast<int32>(pos.x)
		, static_cast<int32>(pos.y), str.c_str()
		, static_cast<int32>(str.size())
	);

	// 5. 원상 복구
	::SetTextColor(hdc, oldColor);
	::SetBkMode(hdc, oldBkMode);
	::SelectObject(hdc, hOldFont);

	// 6. 폰트 삭제 (안 하면 GDI 핸들 누수)
	::DeleteObject(hFont);
}

void Utils::DrawRect(HDC hdc, Pos pos, int32 w, int32 h)
{
	::Rectangle(hdc
		, static_cast<int32>(pos.x - w / 2)
		, static_cast<int32>(pos.y - h / 2)
		, static_cast<int32>(pos.x + w / 2)
		, static_cast<int32>(pos.y + h / 2)
	);
}

void Utils::DrawCircle(HDC hdc, Pos pos, int32 radius)
{
	::Ellipse(hdc, static_cast<int32>(pos.x - radius), static_cast<int32>(pos.y - radius), static_cast<int32>(pos.x + radius), static_cast<int32>(pos.y + radius));
}

void Utils::DrawLine(HDC hdc, Pos from, Pos to)
{
	::MoveToEx(hdc, static_cast<int32>(from.x), static_cast<int32>(from.y), nullptr);
	::LineTo(hdc, static_cast<int32>(to.x), static_cast<int32>(to.y));
}

void Utils::DrawRectColored(HDC hdc, Pos pos, int32 w, int32 h, COLORREF color, bool isAutoPos)
{
	HPEN pen = ::CreatePen(PS_SOLID, 1, color);
	HPEN oldPen = (HPEN)::SelectObject(hdc, (HGDIOBJ)pen);
	HBRUSH brush = ::CreateSolidBrush(color);
	HBRUSH oldBrush = (HBRUSH)::SelectObject(hdc, (HGDIOBJ)brush);

	if (isAutoPos) {
		::Rectangle(hdc,
			static_cast<int32>(pos.x - w / 2),
			static_cast<int32>(pos.y - h / 2),
			static_cast<int32>(pos.x + w / 2),
			static_cast<int32>(pos.y + h / 2));
	}
	else
	{
		static const int32 srtx = static_cast<int32>(w / 2);
		//int widrt = static_cast<int32>(pos.x + w / 2);
		//widrt = static_cast<int32>(widrt * (w / 100));
		::Rectangle(hdc,
			static_cast<int32>(pos.x - srtx),
			static_cast<int32>(pos.y - h / 2),
			static_cast<int32>(pos.x - srtx + w),
			static_cast<int32>(pos.y + h / 2));
	}
	

	::SelectObject(hdc, oldPen);
	::DeleteObject(pen);
	::SelectObject(hdc, oldBrush);
	::DeleteObject(brush);
}


void Utils::DrawLineColored(HDC hdc, Pos from, Pos to, COLORREF color)
{
	HPEN pen = ::CreatePen(PS_SOLID, 1, color);
	HPEN oldPen = (HPEN)::SelectObject(hdc, (HGDIOBJ)pen);

	::MoveToEx(hdc, static_cast<int32>(from.x), static_cast<int32>(from.y), nullptr);
	::LineTo(hdc, static_cast<int32>(to.x), static_cast<int32>(to.y));

	::SelectObject(hdc, oldPen);
	::DeleteObject(pen);
}

void Utils::DrawRectAlpha(HDC hdc, Pos pos, int32 w, int32 h, COLORREF color, BYTE alpha)
{
	HDC memDC = ::CreateCompatibleDC(hdc);
	HBITMAP bmp = ::CreateCompatibleBitmap(hdc, w, h);
	HBITMAP oldBmp = (HBITMAP)::SelectObject(memDC, bmp);

	HBRUSH brush = ::CreateSolidBrush(color);
	RECT rc = { 0, 0, w, h };
	::FillRect(memDC, &rc, brush);
	::DeleteObject(brush);

	BLENDFUNCTION bf = {};
	bf.BlendOp = AC_SRC_OVER;
	bf.SourceConstantAlpha = alpha;

	::AlphaBlend(hdc,
		static_cast<int32>(pos.x - w / 2),
		static_cast<int32>(pos.y - h / 2),
		w, h,
		memDC, 0, 0, w, h, bf);

	::SelectObject(memDC, oldBmp);
	::DeleteObject(bmp);
	::DeleteDC(memDC);
}

void Utils::DrawRectBorder(HDC hdc, Pos pos, int32 w, int32 h, COLORREF color, int32 thickness)
{
	// PS_INSIDEFRAME: keep thick borders inside the rect
	HPEN pen = ::CreatePen(PS_INSIDEFRAME, thickness, color);
	HPEN oldPen = (HPEN)::SelectObject(hdc, (HGDIOBJ)pen);
	// NULL_BRUSH: draw border only, leave inside untouched
	HBRUSH oldBrush = (HBRUSH)::SelectObject(hdc, ::GetStockObject(NULL_BRUSH));

	int32 left = static_cast<int32>(pos.x - w / 2);
	int32 top = static_cast<int32>(pos.y - h / 2);

	::Rectangle(hdc, left, top, left + w, top + h);

	::SelectObject(hdc, oldBrush);
	::SelectObject(hdc, oldPen);
	::DeleteObject(pen);
}

void Utils::ReadBmp(const wstring& path)
{
	FILE* fp = nullptr;

	if (::_wfopen_s(&fp, path.c_str(), L"rb") != 0)
		return;

	BITMAPFILEHEADER fileHeader = {};
	::fread_s(&fileHeader, sizeof(fileHeader), sizeof(fileHeader), 1, fp);

	BITMAPINFOHEADER infoHeader = {};
	::fread_s(&infoHeader, sizeof(infoHeader), sizeof(infoHeader), 1, fp);

	int32 imgSize = infoHeader.biWidth * infoHeader.biHeight * 4;
	char* buffer = (char*)::malloc(imgSize);
	fread_s(buffer, imgSize, imgSize, 1, fp);

	//::free(buffer);
}
