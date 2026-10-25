#include "pch.h"
#include "MiniMap.h"
#include "GameScene.h"
#include "SceneMgr.h"
#include "Actor.h"
#include "Tilemap.h"
#include "ResMgr.h"
#include "Core.h"
#include "MyPlayer.h"

MiniMap::MiniMap()
{
}

MiniMap::~MiniMap()
{
	if (_terrainDC != nullptr)
	{
		::SelectObject(_terrainDC, _prevBitmap);        // 원래 비트맵 되돌리고
		::DeleteObject(_terrainBitmap);                         // 내가 만든 비트맵 삭제
		::DeleteDC(_terrainDC);                                         // 메모리 DC 삭제
	}
}

void MiniMap::BeginPlay()
{
	// 타일맵 가져오기
	_tm = GET(ResMgr)->GetTilemap(L"Tilemap_01");
	if (_tm == nullptr)
		return;

	// BeginPlay가 두 번 불려도 한 번만 생성
	if (_terrainDC != nullptr)
		return;

	Vec2Int mapSize = _tm->GetMapSize();

	// 1. 화면 DC와 호환되는 메모리 DC + 비트맵 생성 (1셀 = 1px)
	HWND hwnd = GET(Core)->GetHwnd();
	HDC hdc = ::GetDC(hwnd);
	_terrainDC = ::CreateCompatibleDC(hdc);
	_terrainBitmap = ::CreateCompatibleBitmap(hdc, mapSize.x, mapSize.y);
	_prevBitmap = (HBITMAP)::SelectObject(_terrainDC, _terrainBitmap);
	::ReleaseDC(hwnd, hdc);

	// 2. 메모리 DC에 타일 색칠
	for (int32 y = 0; y < mapSize.y; y++)
	{
		for (int32 x = 0; x < mapSize.x; x++)
		{
			Tile* tile = _tm->GetTileAt({ x, y });
			if (tile == nullptr)
				continue;

			if (tile->value == 1)
				::SetPixel(_terrainDC, x, y, RGB(60, 55, 50));      // 진입 불가 칸
			else
				::SetPixel(_terrainDC, x, y, RGB(170, 160, 130));   // 이동 가능 칸
		}
	}
}

void MiniMap::Render(HDC hdc)
{
	if (_sprite == nullptr)
		return;

	// 32bit BMP(premultiplied alpha)의 알파 채널로 투명 처리 + 전체 반투명
	BLENDFUNCTION bf = {};
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.SourceConstantAlpha = _alpha;
	bf.AlphaFormat = AC_SRC_ALPHA;

	::SetStretchBltMode(hdc, COLORONCOLOR);

	::AlphaBlend(hdc,
		(int32)_pos.x - _size.x / 2,
		(int32)_pos.y - _size.y / 2,
		_size.x,
		_size.y,
		_sprite->GetDC(),
		_sprite->GetPos().x,
		_sprite->GetPos().y,
		_sprite->GetSize().x,
		_sprite->GetSize().y,
		bf);

	// 지형: 미리 그려둔 비트맵을 미니맵 스크린 크기로 늘려서 복사
	if (_terrainDC != nullptr)
	{
		Vec2Int mapSize = _tm->GetMapSize();
		int32 marginX = _size.x - _scrinSize.x;
		int32 marginY = _size.y - _scrinSize.y;

		::StretchBlt(hdc,
			(int32)_pos.x - _size.x / 2 + marginX / 2,
			(int32)_pos.y - _size.y / 2 + marginY / 2,
			_scrinSize.x,
			_scrinSize.y,
			_terrainDC,
			0, 0,
			mapSize.x, mapSize.y,
			SRCCOPY);
	}

	// 오브잭트 얻어오기
	GameScene* curScene = (GameScene*)GET(SceneMgr)->GetCurrentScene();
	if (curScene != nullptr)
	{
		const float tileSize = 32.f;    // TILE_SIZEX
		const float mapCellX = 100.f;   // Tilemap_01 가로 셀 수
		const float mapCellY = 100.f;   // Tilemap_01 세로 셀 수

		// 셀 1칸이 미니맵에서 몇 px인지
		float ratioX = _scrinSize.x / mapCellX;
		float ratioY = _scrinSize.y / mapCellY;

		MyPlayer* myPlayer = GET(SceneMgr)->GetMyPlayer();

		const vector<Actor*>& actors = curScene->_actors[LAYER_OBJECT];
		for (int32 i = 0; i < actors.size(); i++) {
			Actor* obj = actors[i];

			// 나는 다른 점에 가려지지 않게 루프 끝나고 따로 그림
			if (obj == myPlayer)
				continue;

			int32 px = (int32)(obj->GetPos().x / tileSize * ratioX);
			int32 py = (int32)(obj->GetPos().y / tileSize * ratioY);
			PrintDot(hdc, px, py, 4, RGB(80, 160, 255));
		}

		// 내 플레이어: 노란색, 맨 위에
		if (myPlayer != nullptr)
		{
			int32 px = (int32)(myPlayer->GetPos().x / tileSize * ratioX);
			int32 py = (int32)(myPlayer->GetPos().y / tileSize * ratioY);
			PrintDot(hdc, px, py, 6, RGB(255, 220, 0));
		}
	}
}

void MiniMap::PrintDot(HDC hdc, int32 x, int32 y, int32 size, COLORREF color)
{
	float marginX = _size.x - _scrinSize.x;
	float marginY = _size.x - _scrinSize.y;

	Utils::DrawDot(hdc, 
		{ 
			_pos.x + x - _size.x / 2 + marginX/2,
			_pos.y + y - _size.y / 2 + marginY/2
		},
		size, 
		color);
}
