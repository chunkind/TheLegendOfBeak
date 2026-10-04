#include "pch.h"
#include "SceneMgr.h"
#include "GameScene.h"
#include "EditScene.h"
#include "MapEditScene.h"
#include "MenuScene.h"
#include "MyPlayer.h"

SceneMgr::SceneMgr()
{

}

SceneMgr::~SceneMgr()
{

}

void SceneMgr::Init()
{

}

void SceneMgr::Update()
{
	if (_scene)
		_scene->Update();
}

void SceneMgr::Render(HDC hdc)
{
	if (_scene)
		_scene->Render(hdc);
}

void SceneMgr::Clear()
{
	SAFE_DELETE(_scene);
}

void SceneMgr::ChangeScene(SceneType sceneType)
{
	if (_sceneType == sceneType)
		return;

	Scene* newScene = nullptr;

	switch (sceneType)
	{
	case SceneType::GameScene:
		newScene = new GameScene();
		break;
	case SceneType::EditScene:
		newScene = new EditScene();
		break;
	case SceneType::MapEditScene:
		newScene = new MapEditScene();
		break;
	case SceneType::MenuScene:
		newScene = new MenuScene();
		break;
	}

	SAFE_DELETE(_scene);

	_scene = newScene;
	_sceneType = sceneType;

	newScene->Init();
}

class GameScene* SceneMgr::GetGameScene()
{
	return dynamic_cast<GameScene*>(GetCurrentScene());
}

uint64 SceneMgr::GetMyPlayerId()
{
	return _myPlayer ? _myPlayer->info.objectid() : 0;
}
