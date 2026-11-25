#pragma once

#include "UI.h"

struct ShopItem
{
	Protocol::ITEM_TYPE itemType = Protocol::ITEM_TYPE_NONE;
	wstring name;
	int32 price = 0;
	wstring desc;
};

class ShopUI : public UI
{
	using Super = UI;
public:
	ShopUI();
	virtual ~ShopUI() override;

	virtual void Tick() override;
	virtual void Render(HDC hdc) override;

	void Open(const wstring& title, const vector<ShopItem>& items);
	void Close();
	bool IsOpen() { return IsVisible(); }

	// 서버의 구매 결과(S_Buy) 반영
	void OnBuyResult(bool success, Protocol::ITEM_TYPE itemType);

private:
	const ShopItem* FindItem(Protocol::ITEM_TYPE itemType);

private:
	wstring _resultMsg;					// 구매 결과 메시지
	COLORREF _resultColor = RGB(255, 255, 255);
	bool _waitingBuy = false;			// 응답 대기 중 중복 요청 방지

	wstring _title;
	vector<ShopItem> _items;
	int32 _selectedIdx = 0;

	// 상점을 연 Space 입력이 같은 프레임에 상점 입력으로 처리되지 않도록 한 프레임 무시
	bool _skipInputOnce = false;
};
