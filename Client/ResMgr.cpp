#include "pch.h"
#include "ResMgr.h"
#include "Texture.h"
#include "Sprite.h"
#include "Flipbook.h"
#include "Tilemap.h"
#include "Sound.h"
#include "Video.h"

ResMgr::ResMgr()
{

}

ResMgr::~ResMgr()
{
	Clear();
}

void ResMgr::Init(HWND hwnd, fs::path resourcePath)
{
	_hwnd = hwnd;
	_resourcePath = resourcePath;

	//fs::current_path();
	//_resourcePath.relative_path();
	//fs::absolute(_resourcePath);
}

void ResMgr::Clear()
{
	for (auto& item : _textures)
		DEL(item.second);

	_textures.clear();

	for (auto& item : _sprites)
		DEL(item.second);

	_sprites.clear();

	for (auto& item : _flipbooks)
		DEL(item.second);

	_flipbooks.clear();

	for (auto& item : _tilemaps)
		DEL(item.second);

	_tilemaps.clear();

	for (auto& item : _sounds)
		DEL(item.second);

	_sounds.clear();
}

Texture* ResMgr::LoadTexture(const wstring& key, const wstring& path, uint32 transparent /*= RGB(255, 0, 255)*/)
{
	if (_textures.find(key) != _textures.end())
		return _textures[key];

	fs::path fullPath = _resourcePath / path;

	Texture* texture = new Texture();
	texture->LoadBmp(_hwnd, fullPath.c_str());
	texture->SetTransparent(transparent);
	_textures[key] = texture;

	return texture;
}

Sprite* ResMgr::CreateSprite(const wstring& key, Texture* texture, int32 x, int32 y, int32 cx, int32 cy)
{
	if (_sprites.find(key) != _sprites.end())
		return _sprites[key];

	if (cx == 0)
		cx = texture->GetSize().x;

	if (cy == 0)
		cy = texture->GetSize().y;

	Sprite* sprite = new Sprite(texture, x, y, cx, cy);
	_sprites[key] = sprite;

	return sprite;
}

Flipbook* ResMgr::CreateFlipbook(const wstring& key)
{
	if (_flipbooks.find(key) != _flipbooks.end())
		return _flipbooks[key];

	Flipbook* fb = new Flipbook();
	_flipbooks[key] = fb;

	return fb;
}

Tilemap* ResMgr::CreateTilemap(const wstring& key)
{
	if (_tilemaps.find(key) != _tilemaps.end())
		return _tilemaps[key];

	Tilemap* tm = new Tilemap();
	_tilemaps[key] = tm;

	return tm;
}

void ResMgr::SaveTilemap(const wstring& key, const wstring& path)
{
	Tilemap* tilemap = GetTilemap(key);

	fs::path fullPath = _resourcePath / path;
	tilemap->SaveFile(fullPath);
}

Tilemap* ResMgr::LoadTilemap(const wstring& key, const wstring& path)
{
	Tilemap* tilemap = nullptr;

	if (_tilemaps.find(key) == _tilemaps.end())
		_tilemaps[key] = new Tilemap();

	tilemap = _tilemaps[key];

	fs::path fullPath = _resourcePath / path;
	tilemap->LoadFile(fullPath);

	return tilemap;
}

Sound* ResMgr::LoadSound(const wstring& key, const wstring& path)
{
	if (_sounds.find(key) != _sounds.end())
		return _sounds[key];

	fs::path fullPath = _resourcePath / path;

	Sound* sound = new Sound();
	sound->LoadWave(fullPath);
	_sounds[key] = sound;

	return sound;
}

Video* ResMgr::LoadVideo(const wstring& key, const wstring& path)
{
	Video* video = new Video();
	video->Load(path);

	wstring alias = L"";
	wstring pullPath = video->getFullPath();

	if (key.empty())
		alias = video->GetAlias();
	else
		alias = key;

	_videos[alias] = video;

	return video;
}
