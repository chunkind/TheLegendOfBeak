#include "pch.h"
#include "MenuScene.h"
#include "ResMgr.h"
#include "Sprite.h"
#include "SpriteActor.h"

MenuScene::MenuScene()
{

}

MenuScene:: ~MenuScene()
{

}

void MenuScene::Init()
{
	Super::Init();

	GET(ResMgr)->LoadTexture(L"UI_Main", L"Sprite\\UI\\UI_Main.bmp");

	GET(ResMgr)->CreateSprite(L"UI_Main", GET(ResMgr)->GetTexture(L"UI_Main"));

	//Load
	_background = GET(ResMgr)->GetSprite(L"UI_Main");

	//SpriteActor* background = new SpriteActor();
	//background->SetSprite(_background);
	//background->SetLayer(LAYER_BACKGROUND);
	//const Vec2Int size = _background->GetSize();
	//background->SetPos(Vec2(size.x / 2, size.y / 2));

	//Super::AddActor(background);
}

void MenuScene::Update()
{

}

void MenuScene::Render(HDC hdc)
{
	Super::Render(hdc);

	if (_background)
	{
		Vec2Int pos = _background->GetPos();
		Vec2Int size = _background->GetSize();

		::SetStretchBltMode(hdc, HALFTONE);
		::SetBrushOrgEx(hdc, 0, 0, nullptr);   // HALFTONE 사용 시 권장

		::StretchBlt(hdc,
			0, 0, GWinSizeX, GWinSizeY,         // 화면 전체 크기로
			_background->GetDC(),
			pos.x, pos.y, size.x, size.y,       // 원본 이미지 전체를
			SRCCOPY);
	}

	int32 w = 400;
	int32 h = 200;

	float posX = GWinSizeX / 2;
	float posY = GWinSizeY / 2;

	//Utils::DrawRectAlpha(hdc, { posX, posY }, w, h, RGB(0, 0, 0), 0);
	Utils::DrawRectBorder(hdc, { posX, posY }, w, h, RGB(255, 255, 255), 2);
	Utils::DrawTextW(hdc, { posX - w/2, posY }, L"게임 시작");

	posY += 20;

	Utils::DrawTextW(hdc, { posX - w / 2, posY }, L"맵 제작");
}