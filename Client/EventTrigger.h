#pragma once

#include "GameObject.h"
#include "ShopUI.h"

class Sprite;

class EventTrigger : public GameObject
{
	using Super = GameObject;
public:
	EventTrigger();
	virtual ~EventTrigger() override;

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc) override;

	void SetSprite(Sprite* sprite) { _sprite = sprite; }
	void StartEvent() { _isStart = true; }

	void SetSize(Vec2Int size) { _size = size; }
	Vec2Int GetSize() { return _size; }

	void SetOpener(GameObject* opener) { _opener = opener; }
	GameObject* GetOpener() { return _opener; }

	// 대사가 끝난 뒤 열 상점 정보 (목록이 비어 있으면 상점 없이 이벤트 종료)
	void SetShop(const wstring& title, const vector<ShopItem>& items) { _shopTitle = title; _shopItems = items; }

private:
	bool OpenShop();
	void EndEvent();

protected:
	wstring _shopTitle;
	vector<ShopItem> _shopItems;
	bool _isShopOpen = false;

	GameObject* _opener = nullptr;
	Sprite* _sprite = nullptr;
	bool _isStart = false;
	bool _isRender = false;
	Vec2Int _size = { 100, 100 };
};
