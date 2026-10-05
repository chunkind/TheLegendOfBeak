#include "pch.h"
#include "MenuScene.h"
#include "ResMgr.h"
#include "InputMgr.h"
#include "Sprite.h"
#include "SpriteActor.h"
#include "SceneMgr.h"
#include "NetMgr.h"

MenuScene::MenuScene()
{

}

MenuScene:: ~MenuScene()
{

}

void MenuScene::Init()
{
	Super::Init();

	_boxPosX = GWinSizeX / 2;
	_boxPosY = GWinSizeY / 2 + 140;

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
	if (GET(InputMgr)->GetButton(KeyType::W))
	{
		if (!_keyPress)
		{
			_selMenuNumber--;
			if (_selMenuNumber < 0)
				_selMenuNumber = 1;

			_boxPosY = _menuNumbers[_selMenuNumber];
			_keyPress = true;
		}
	}
	else if (GET(InputMgr)->GetButton(KeyType::S))
	{
		if (!_keyPress)
		{
			_selMenuNumber++;
			if (_selMenuNumber > 1)
				_selMenuNumber = 0;
			
			_boxPosY = _menuNumbers[_selMenuNumber];
			_keyPress = true;
		}
	}
	else if (GET(InputMgr)->GetButton(KeyType::Enter))
	{
		switch (_selMenuNumber)
		{
		case 0:
			GET(SceneMgr)->ChangeScene(SceneType::GameScene);
			break;
		case 1:
			GET(SceneMgr)->ChangeScene(SceneType::EditScene);
			break;
		}
	}
	else
	{
		_keyPress = false;
	}
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

	int32 w = 160;
	int32 h = 50;

	float textPosX = GWinSizeX / 2;
	float textPosY = GWinSizeY / 2 + 140;

	//Utils::DrawRectAlpha(hdc, { posX, posY }, w, h, RGB(0, 0, 0), 0);
	Utils::DrawRectBorder(hdc, { _boxPosX, _boxPosY }, w, h, RGB(255, 255, 255), 2);
	Utils::DrawTextW(hdc, { textPosX - 40, textPosY - 10}, L"게임 시작", true);
	textPosY += 40;
	Utils::DrawTextW(hdc, { textPosX - 40, textPosY - 10}, L"맵 제작", true);
}