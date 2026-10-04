#pragma once

#include "Scene.h"

class Sprite;

class MenuScene :public Scene
{
	using Super = Scene;

public:
	MenuScene();
	virtual ~MenuScene() override;

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

private:
	Sprite* _background = nullptr;
};
