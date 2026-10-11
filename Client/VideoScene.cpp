#include "pch.h"
#include "VideoScene.h"
#include "Core.h"
#include "ResMgr.h"
#include "Video.h"
#include "TimeMgr.h"
#include "SceneMgr.h"
#include "InputMgr.h"

VideoScene::VideoScene()
{
	
}

VideoScene::~VideoScene()
{
	
}

void VideoScene::Init()
{
	Video* video = GET(ResMgr)->LoadVideo(L"", L"C:\\git\\TheLegendOfBeak\\Resources\\Media\\intro.wmv");

	_currentVideo = video;


}

void VideoScene::Update()
{
	// Update
	_waitTime += DT;
	if (_waitTime < 1.5f)   // 0.5초 동안은 대기
		return;

	if (_currentVideo->IsPlay() == false)
		_currentVideo->Play();
	if (_currentVideo->IsEnd() == false)
		_currentVideo->CheckEnd();

	// 신 전환
	if (_currentVideo->IsEnd())
	{
		GET(SceneMgr)->ChangeScene(SceneType::MenuScene);
		return;
	}

	if (GET(InputMgr)->GetButtonDown(KeyType::Enter))
	{
		_currentVideo->Stop();
		GET(SceneMgr)->ChangeScene(SceneType::MenuScene);
		return;
	}
}

void VideoScene::Render(HDC hdc)
{
	
}

