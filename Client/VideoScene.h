#pragma once

#include "Scene.h"

class Video;

class VideoScene : public Scene
{
public:
	VideoScene();
	virtual ~VideoScene() override;

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

private:
	Video* _currentVideo;
	float _waitTime = 0.f;
};

