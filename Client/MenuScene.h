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
	float _boxPosX = 0;
	float _boxPosY = 0;
	bool _keyPress = false;
	int32 _selMenuNumber = 0;
	int32 _menuNumbers[2] = { GWinSizeY / 2 + 140 , GWinSizeY / 2 + 140  + 45};
};
