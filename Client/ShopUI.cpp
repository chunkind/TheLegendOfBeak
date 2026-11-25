#include "pch.h"
#include "ShopUI.h"
#include "InputMgr.h"
#include "SceneMgr.h"
#include "MyPlayer.h"
#include "NetMgr.h"
#include "ClientPacketHandler.h"

namespace
{
	const int32 HEADER_HEIGHT = 50;		// 제목 영역 높이
	const int32 ROW_HEIGHT = 40;		// 목록 한 줄 높이
	const int32 FOOTER_HEIGHT = 80;		// 설명 + 하단 영역 높이
	const int32 PADDING = 20;			// 창 안쪽 여백
}

ShopUI::ShopUI()
{
	_size = { 480, 360 };
	SetVisible(false);
}

ShopUI::~ShopUI()
{
}

void ShopUI::Open(const wstring& title, const vector<ShopItem>& items)
{
	_title = title;
	_items = items;
	_selectedIdx = 0;
	_skipInputOnce = true;
	_waitingBuy = false;
	_resultMsg.clear();
	SetVisible(true);
}

void ShopUI::OnBuyResult(bool success, Protocol::ITEM_TYPE itemType)
{
	_waitingBuy = false;

	const ShopItem* item = FindItem(itemType);
	const wstring name = item ? item->name : L"아이템";

	if (success)
	{
		_resultMsg = std::format(L"{}을(를) 구매했습니다.", name);
		_resultColor = RGB(120, 230, 120);
	}
	else
	{
		_resultMsg = L"골드가 부족합니다.";
		_resultColor = RGB(255, 110, 110);
	}
}

const ShopItem* ShopUI::FindItem(Protocol::ITEM_TYPE itemType)
{
	for (const ShopItem& item : _items)
	{
		if (item.itemType == itemType)
			return &item;
	}
	return nullptr;
}

void ShopUI::Close()
{
	SetVisible(false);
}

void ShopUI::Tick()
{
	if (IsVisible() == false)
		return;

	if (_skipInputOnce)
	{
		_skipInputOnce = false;
		return;
	}

	if (GET(InputMgr)->GetButtonDown(KeyType::Esc))
	{
		Close();
		return;
	}

	if (_items.empty())
		return;

	const int32 count = static_cast<int32>(_items.size());

	if (GET(InputMgr)->GetButtonDown(KeyType::W) || GET(InputMgr)->GetButtonDown(KeyType::Up))
	{
		_selectedIdx = (_selectedIdx - 1 + count) % count;
		_resultMsg.clear();
	}
	else if (GET(InputMgr)->GetButtonDown(KeyType::S) || GET(InputMgr)->GetButtonDown(KeyType::Down))
	{
		_selectedIdx = (_selectedIdx + 1) % count;
		_resultMsg.clear();
	}

	// 구매 요청: 골드 차감은 서버가 판정하고 S_Buy로 결과를 받는다
	if (GET(InputMgr)->GetButtonDown(KeyType::SpaceBar) && _waitingBuy == false)
	{
		const ShopItem& item = _items[_selectedIdx];
		GET(NetMgr)->RegisterPacket(ClientPacketHandler::Make_C_Buy(item.itemType));
		_waitingBuy = true;
	}
}

void ShopUI::Render(HDC hdc)
{
	if (IsVisible() == false)
		return;

	const int32 left = static_cast<int32>(_pos.x) - _size.x / 2;
	const int32 top = static_cast<int32>(_pos.y) - _size.y / 2;
	const int32 right = left + _size.x;
	const int32 bottom = top + _size.y;

	// 배경 + 테두리
	Utils::DrawRectAlpha(hdc, _pos, _size.x, _size.y, RGB(20, 20, 30), 220);
	Utils::DrawRectBorder(hdc, _pos, _size.x, _size.y, RGB(200, 180, 120), 2);

	// ① 제목
	Utils::DrawText(hdc, { (float)(left + PADDING), (float)(top + 14) }, _title, true, 20, RGB(255, 220, 120));
	Utils::DrawLineColored(hdc,
		{ (float)(left + 10), (float)(top + HEADER_HEIGHT) },
		{ (float)(right - 10), (float)(top + HEADER_HEIGHT) },
		RGB(120, 110, 90));

	// ② 목록
	const int32 listTop = top + HEADER_HEIGHT + 10;
	for (int32 i = 0; i < static_cast<int32>(_items.size()); i++)
	{
		const ShopItem& item = _items[i];
		const int32 rowTop = listTop + i * ROW_HEIGHT;
		const int32 rowCenterY = rowTop + ROW_HEIGHT / 2;

		// 선택 줄 강조
		if (i == _selectedIdx)
		{
			Utils::DrawRectAlpha(hdc,
				{ (float)(left + _size.x / 2), (float)rowCenterY },
				_size.x - 20, ROW_HEIGHT - 4, RGB(255, 255, 255), 50);
			Utils::DrawText(hdc, { (float)(left + PADDING), (float)(rowTop + 10) }, L"▶", true, 16, RGB(255, 220, 120));
		}

		Utils::DrawText(hdc, { (float)(left + PADDING + 30), (float)(rowTop + 10) }, item.name, false, 16);
		Utils::DrawText(hdc, { (float)(right - PADDING), (float)(rowTop + 10) },
			std::format(L"{} G", item.price), false, 16, RGB(255, 220, 120), true);
	}

	// ③ 설명
	const int32 footerTop = bottom - FOOTER_HEIGHT;
	Utils::DrawLineColored(hdc,
		{ (float)(left + 10), (float)footerTop },
		{ (float)(right - 10), (float)footerTop },
		RGB(120, 110, 90));

	if (_resultMsg.empty() == false)
		Utils::DrawText(hdc, { (float)(left + PADDING), (float)(footerTop + 10) }, _resultMsg, true, 14, _resultColor);
	else if (_selectedIdx >= 0 && _selectedIdx < static_cast<int32>(_items.size()))
		Utils::DrawText(hdc, { (float)(left + PADDING), (float)(footerTop + 10) }, _items[_selectedIdx].desc, false, 14, RGB(220, 220, 220));

	// ④ 보유 골드 + 조작 안내
	int32 money = 0;
	if (MyPlayer* myPlayer = GET(SceneMgr)->GetMyPlayer())
		money = myPlayer->info.money();

	Utils::DrawText(hdc, { (float)(left + PADDING), (float)(bottom - 30) },
		std::format(L"보유 골드: {} G", money), true, 14, RGB(255, 220, 120));
	Utils::DrawText(hdc, { (float)(right - PADDING), (float)(bottom - 30) },
		L"W/S 선택   Space 구매   Esc 닫기", false, 14, RGB(180, 180, 180), true);
}
