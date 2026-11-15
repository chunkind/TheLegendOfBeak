#include "pch.h"
#include "GameResources.h"
#include "ResMgr.h"
#include "Flipbook.h"

namespace
{
	// 플립북 생성 헬퍼: 텍스처 키로 텍스처를 찾아 플립북을 만들고 정보를 채운다
	Flipbook* CreateFlipbook(const wstring& key, const wstring& textureKey, Vec2Int size, int32 start, int32 end, int32 line, float duration, bool loop = true)
	{
		Flipbook* fb = GET(ResMgr)->CreateFlipbook(key);
		fb->SetInfo({ GET(ResMgr)->GetTexture(textureKey), key, size, start, end, line, duration, loop });
		return fb;
	}
}

namespace GameResources
{
	// Load everything: textures -> sprites -> flipbooks -> sounds
	// ResMgr returns the existing resource when a key is already registered,
	// so calling this more than once is safe.
	void LoadAll()
	{
		LoadTextures();
		LoadSprites();

		LoadPlayer();
		LoadNpc();
		LoadMonster();
		LoadObjects();
		LoadProjectiles();
		LoadEffect();

		LoadSounds();
	}

	void LoadSounds()
	{
		GET(ResMgr)->LoadSound(L"BGM", L"Sound\\BGM.wav");
		GET(ResMgr)->LoadSound(L"Attack", L"Sound\\Sword.wav");
	}

	void LoadTextures()
	{
		GET(ResMgr)->LoadTexture(L"Stage01", L"Sprite\\Map\\main01.bmp");
		GET(ResMgr)->LoadTexture(L"Tile", L"Sprite\\Map\\tile.bmp", RGB(255, 0, 255));
		GET(ResMgr)->LoadTexture(L"Sword", L"Sprite\\Item\\Sword.bmp");
		GET(ResMgr)->LoadTexture(L"Arrow", L"Sprite\\Item\\Arrow.bmp", RGB(255, 0, 255));
		GET(ResMgr)->LoadTexture(L"Potion", L"Sprite\\UI\\Mp.bmp");
		GET(ResMgr)->LoadTexture(L"Player", L"Sprite\\Player\\player.bmp", RGB(255, 0, 255));
		GET(ResMgr)->LoadTexture(L"StoreNcpIdle", L"Sprite\\Player\\lady001_idle.bmp");
		GET(ResMgr)->LoadTexture(L"StoreNcpMove", L"Sprite\\Player\\lady001_move.bmp");
		GET(ResMgr)->LoadTexture(L"Dack", L"Sprite\\Monster\\dack.bmp", RGB(255, 0, 255));
		GET(ResMgr)->LoadTexture(L"Hit", L"Sprite\\Effect\\Hit.bmp", RGB(0, 0, 0));
		// UI
		GET(ResMgr)->LoadTexture(L"Start", L"Sprite\\UI\\Start.bmp");
		GET(ResMgr)->LoadTexture(L"Edit", L"Sprite\\UI\\Edit.bmp");
		GET(ResMgr)->LoadTexture(L"Exit", L"Sprite\\UI\\Exit.bmp");
		GET(ResMgr)->LoadTexture(L"RectBar", L"Sprite\\UI\\ui_hp_mp_bar_a.bmp");
		GET(ResMgr)->LoadTexture(L"MiniMap", L"Sprite\\UI\\ui_minimap_a.bmp");
		GET(ResMgr)->LoadTexture(L"MsgBox", L"Sprite\\UI\\ui_massage_box_a.bmp");
		//Building
		GET(ResMgr)->LoadTexture(L"PosionShop", L"Sprite\\Building\\posion_shop_a.bmp");
		GET(ResMgr)->LoadTexture(L"Tree", L"Sprite\\Map\\tree.bmp", RGB(255, 0, 255));
		GET(ResMgr)->LoadTexture(L"Tree01", L"Sprite\\Map\\tree_01.bmp", RGB(255, 0, 255));
	}

	void LoadSprites()
	{
		GET(ResMgr)->CreateSprite(L"Stage01", GET(ResMgr)->GetTexture(L"Stage01"));
		GET(ResMgr)->CreateSprite(L"TileO", GET(ResMgr)->GetTexture(L"Tile"), 0, 0, 32, 32);
		GET(ResMgr)->CreateSprite(L"TileX", GET(ResMgr)->GetTexture(L"Tile"), 32, 0, 32, 32);
		GET(ResMgr)->CreateSprite(L"Start_Off", GET(ResMgr)->GetTexture(L"Start"), 0, 0, 150, 150);
		GET(ResMgr)->CreateSprite(L"Start_On", GET(ResMgr)->GetTexture(L"Start"), 150, 0, 150, 150);
		GET(ResMgr)->CreateSprite(L"Edit_Off", GET(ResMgr)->GetTexture(L"Edit"), 0, 0, 150, 150);
		GET(ResMgr)->CreateSprite(L"Edit_On", GET(ResMgr)->GetTexture(L"Edit"), 150, 0, 150, 150);
		GET(ResMgr)->CreateSprite(L"Exit_Off", GET(ResMgr)->GetTexture(L"Exit"), 0, 0, 150, 150);
		GET(ResMgr)->CreateSprite(L"Exit_On", GET(ResMgr)->GetTexture(L"Exit"), 150, 0, 150, 150);
		GET(ResMgr)->CreateSprite(L"HpBar", GET(ResMgr)->GetTexture(L"RectBar"), 0, 32 * 0, 32 * 4, 32);
		GET(ResMgr)->CreateSprite(L"MpBar", GET(ResMgr)->GetTexture(L"RectBar"), 0, 32 * 1, 32 * 4, 32);
		GET(ResMgr)->CreateSprite(L"MoneyBar", GET(ResMgr)->GetTexture(L"RectBar"), 0, 32 * 2, 32 * 4, 32);
		GET(ResMgr)->CreateSprite(L"MiniMap", GET(ResMgr)->GetTexture(L"MiniMap"), 0, 0, 32 * 8, 32 * 8);
		GET(ResMgr)->CreateSprite(L"MsgBox", GET(ResMgr)->GetTexture(L"MsgBox"));
		// Building
		GET(ResMgr)->CreateSprite(L"PosionShopTop", GET(ResMgr)->GetTexture(L"PosionShop"), 0, 0, 32 * 6, 32 * 4);
		GET(ResMgr)->CreateSprite(L"PosionShopDown", GET(ResMgr)->GetTexture(L"PosionShop"), 32 * 0, 32 * 4, 32 * 6, 32 * 2);

		// 지형지물
		GET(ResMgr)->CreateSprite(L"TreeDead01", GET(ResMgr)->GetTexture(L"Tree"), 0, 0, 64, 64);
		GET(ResMgr)->CreateSprite(L"TreeDead02", GET(ResMgr)->GetTexture(L"Tree"), 0, 64, 64, 64);
		GET(ResMgr)->CreateSprite(L"TreeDead03", GET(ResMgr)->GetTexture(L"Tree"), 0, 64 * 2, 64, 64);
		GET(ResMgr)->CreateSprite(L"TreeDead04", GET(ResMgr)->GetTexture(L"Tree"), 0, 64 * 3, 64, 64);

		// 나무
		GET(ResMgr)->CreateSprite(L"Tree01", GET(ResMgr)->GetTexture(L"Tree01"), 0, 0, 32 * 4, 32 * 4);
	}

	void LoadPlayer()
	{
		const Vec2Int size = { 32 * 2, 32 * 2 };

		// IDLE
		CreateFlipbook(L"FB_IdleUp",    L"Player", size, 0, 6, 0, 0.5f);
		CreateFlipbook(L"FB_IdleDown",  L"Player", size, 0, 6, 1, 0.5f);
		CreateFlipbook(L"FB_IdleLeft",  L"Player", size, 0, 6, 2, 0.5f);
		CreateFlipbook(L"FB_IdleRight", L"Player", size, 0, 6, 3, 0.5f);
		// MOVE
		CreateFlipbook(L"FB_MoveUp",    L"Player", size, 0, 6, 4, 0.5f);
		CreateFlipbook(L"FB_MoveDown",  L"Player", size, 0, 6, 5, 0.5f);
		CreateFlipbook(L"FB_MoveLeft",  L"Player", size, 0, 6, 6, 0.5f);
		CreateFlipbook(L"FB_MoveRight", L"Player", size, 0, 6, 7, 0.5f);
		// SKILL
		CreateFlipbook(L"FB_AttackUp",    L"Player", size, 0, 6, 8,  0.5f, false);
		CreateFlipbook(L"FB_AttackDown",  L"Player", size, 0, 6, 9,  0.5f, false);
		CreateFlipbook(L"FB_AttackLeft",  L"Player", size, 0, 6, 10, 0.5f, false);
		CreateFlipbook(L"FB_AttackRight", L"Player", size, 0, 6, 11, 0.5f, false);
		// BOW
		CreateFlipbook(L"FB_BowUp",    L"Player", size, 0, 6, 12, 0.5f, false);
		CreateFlipbook(L"FB_BowDown",  L"Player", size, 0, 6, 13, 0.5f, false);
		CreateFlipbook(L"FB_BowLeft",  L"Player", size, 0, 6, 14, 0.5f, false);
		CreateFlipbook(L"FB_BowRight", L"Player", size, 0, 6, 15, 0.5f, false);
	}

	void LoadNpc()
	{
		const Vec2Int size = { 64, 64 };

		// IDLE
		CreateFlipbook(L"FB_StoreNcpIdleUp",    L"StoreNcpIdle", size, 0, 6, 0, 0.5f);
		CreateFlipbook(L"FB_StoreNcpIdleDown",  L"StoreNcpIdle", size, 0, 6, 1, 0.5f);
		CreateFlipbook(L"FB_StoreNcpIdleLeft",  L"StoreNcpIdle", size, 0, 6, 2, 0.5f);
		CreateFlipbook(L"FB_StoreNcpIdleRight", L"StoreNcpIdle", size, 0, 6, 3, 0.5f);
		// MOVE
		CreateFlipbook(L"FB_StoreNcpMoveUp",    L"StoreNcpMove", size, 0, 6, 0, 0.5f);
		CreateFlipbook(L"FB_StoreNcpMoveDown",  L"StoreNcpMove", size, 0, 6, 1, 0.5f);
		CreateFlipbook(L"FB_StoreNcpMoveLeft",  L"StoreNcpMove", size, 0, 6, 2, 0.5f);
		CreateFlipbook(L"FB_StoreNcpMoveRight", L"StoreNcpMove", size, 0, 6, 3, 0.5f);
	}

	void LoadMonster()
	{
		const Vec2Int size = { 32 * 2, 32 * 2 };

		// MOVE
		CreateFlipbook(L"FB_SnakeUp",    L"Dack", size, 0, 3, 0, 0.5f);
		CreateFlipbook(L"FB_SnakeDown",  L"Dack", size, 0, 3, 1, 0.5f);
		CreateFlipbook(L"FB_SnakeLeft",  L"Dack", size, 0, 3, 2, 0.5f);
		CreateFlipbook(L"FB_SnakeRight", L"Dack", size, 0, 3, 3, 0.5f);
	}

	void LoadObjects()
	{
		const Vec2Int size = { 32 * 2, 32 * 2 };

		CreateFlipbook(L"FB_TreeDead", L"Tree", size, 0, 4, 0, 1.0f);
		CreateFlipbook(L"FB_TreeIdle", L"Tree", size, 1, 4, 0, 1.0f);
	}

	void LoadProjectiles()
	{
		const Vec2Int size = { 32, 32 };

		// MOVE
		CreateFlipbook(L"FB_ArrowUp",    L"Arrow", size, 0, 0, 3, 0.5f);
		CreateFlipbook(L"FB_ArrowDown",  L"Arrow", size, 0, 0, 0, 0.5f);
		CreateFlipbook(L"FB_ArrowLeft",  L"Arrow", size, 0, 0, 1, 0.5f);
		CreateFlipbook(L"FB_ArrowRight", L"Arrow", size, 0, 0, 2, 0.5f);
	}

	void LoadEffect()
	{
		CreateFlipbook(L"FB_Hit", L"Hit", { 50, 47 }, 0, 5, 0, 0.5f, false);
	}
}
