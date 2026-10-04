#pragma once

enum class SceneType
{
	None,
	GameScene,
	EditScene,
	MapEditScene,
	MenuScene,
};

enum LAYER_TYPE
{
	LAYER_BACKGROUND,
	LAYER_OBJECT,
	LAYER_EFFECT,
	// ...
	LAYER_UI,

	LAYER_MAXCOUNT
};

enum class ColliderType
{
	Box,
	Sphere,
};

enum COLLISION_LAYER_TYPE : uint8
{
	CLT_OBJECT,
	CLT_GROUND,
	CLT_WALL,
	// ...
};

#include "Enum.pb.h"
using Dir = Protocol::DIR_TYPE;

#define DIR_DOWN	Protocol::DIR_TYPE_DOWN
#define DIR_UP		Protocol::DIR_TYPE_UP
#define DIR_LEFT	Protocol::DIR_TYPE_LEFT
#define DIR_RIGHT	Protocol::DIR_TYPE_RIGHT

#define IDLE	Protocol::OBJECT_STATE_TYPE_IDLE
#define MOVE	Protocol::OBJECT_STATE_TYPE_MOVE
#define SKILL	Protocol::OBJECT_STATE_TYPE_SKILL
#define DEAD	Protocol::OBJECT_STATE_TYPE_DEAD

using ObjectState = Protocol::OBJECT_STATE_TYPE;

using WeaponType = Protocol::OBJECT_WEAPON_TYPE;