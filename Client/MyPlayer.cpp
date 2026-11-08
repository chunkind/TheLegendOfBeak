#include "pch.h"
#include "MyPlayer.h"
#include "InputMgr.h"
#include "TimeMgr.h"
#include "ResMgr.h"
#include "Flipbook.h"
#include "CameraComponent.h"
#include "SceneMgr.h"
#include "GameScene.h"
#include "Arrow.h"
#include "HitEffect.h"
#include "EventTrigger.h"

MyPlayer::MyPlayer()
{
	CameraComponent* camera = new CameraComponent();
	AddComponent(camera);
}

MyPlayer::~MyPlayer()
{

}

void MyPlayer::BeginPlay()
{
	Super::BeginPlay();

}

void MyPlayer::Tick()
{
	Super::Tick();

	if (info.state() == Protocol::OBJECT_STATE_TYPE_DEAD || info.hp() <= 0) {
		SendBufferRef send = ClientPacketHandler::Make_C_RemoveObject();
		GET(NetMgr)->RegisterPacket(send);
	}
}

void MyPlayer::Render(HDC hdc)
{
	Super::Render(hdc);
}

void MyPlayer::TickInput()
{
	_keyPressed = true;

	if (GET(InputMgr)->GetButton(KeyType::W))
	{
		SetDir(DIR_UP);
	}
	else if (GET(InputMgr)->GetButton(KeyType::S))
	{
		SetDir(DIR_DOWN);
	}
	else if (GET(InputMgr)->GetButton(KeyType::A))
	{
		SetDir(DIR_LEFT);
	}
	else if (GET(InputMgr)->GetButton(KeyType::D))
	{
		SetDir(DIR_RIGHT);
	}
	else
	{
		_keyPressed = false;
	}

	if (GET(InputMgr)->GetButtonDown(KeyType::KEY_1))
	{
		SetWeaponType(WeaponType::OBJECT_WEAPON_TYPE_SWORD);
		GET(NetMgr)->RegisterPacket(ClientPacketHandler::Make_C_ObjectWeaponChange(info.objectid(), WeaponType::OBJECT_WEAPON_TYPE_SWORD));
	}
	else if (GET(InputMgr)->GetButtonDown(KeyType::KEY_2))
	{
		SetWeaponType(WeaponType::OBJECT_WEAPON_TYPE_BOW);
		GET(NetMgr)->RegisterPacket(ClientPacketHandler::Make_C_ObjectWeaponChange(info.objectid(), WeaponType::OBJECT_WEAPON_TYPE_BOW));
	}

	// 디버그 모드
	if (GET(InputMgr)->GetButtonUp(KeyType::P)) {
		GameScene* scene = dynamic_cast<GameScene*>(GET(SceneMgr)->GetCurrentScene());
		if (scene == nullptr)
			return;

		scene->ToggleDebugger();
	}

	if (GET(InputMgr)->GetButtonDown(KeyType::SpaceBar))
	{
		GameScene* scene = dynamic_cast<GameScene*>(GET(SceneMgr)->GetCurrentScene());
		if (scene == nullptr)
			return;

		// 이벤트
		vector<Actor*> events = scene->_actors[LAYER_EVENT];
		for (int i = 0; i < events.size(); i++)
		{
			EventTrigger* event = static_cast<EventTrigger*>(events[i]);
			Vec2 pos = event->GetPos();
			int posCellPosX = pos.x / 32;
			int posCellPosY = pos.y / 32;
			Vec2Int myPos = GetCellPos();
			if (posCellPosX == myPos.x && posCellPosY == myPos.y) {
				event->StartEvent();
			}
		}

		if (_weaponType == WeaponType::OBJECT_WEAPON_TYPE_SWORD)
		{
			Creature* creature = scene->GetCreatureAt(GetFrontCellPos());
			SetState(SKILL);

			if (creature)
			{
				Vec2Int frontPos = GetFrontCellPos();
				GameObject* target = scene->GetGameObjectAt(frontPos);
				if (target != nullptr)
				{
					GET(NetMgr)->RegisterPacket(ClientPacketHandler::Make_C_Attack(target));
				}
			}
		}
		else if (_weaponType == WeaponType::OBJECT_WEAPON_TYPE_BOW)
		{
			Creature* target = scene->GetCreatureAt(GetLineCellPos());
			SetState(SKILL);
			GET(NetMgr)->RegisterPacket(ClientPacketHandler::Make_C_Attack_Arrow(target));
		}
		
	}
}

void MyPlayer::TryMove()
{
	if (_keyPressed == false)
		return;

	Vec2Int deltaXY[4] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

	if (info.dir() == DIR_UP)
	{
		Vec2Int nextPos = GetCellPos() + deltaXY[info.dir()];
		if (CanGo(nextPos))
		{
			SetCellPos(nextPos);
			SetState(MOVE);
		}
	}
	else  if (info.dir() == DIR_DOWN)
	{
		Vec2Int nextPos = GetCellPos() + deltaXY[info.dir()];
		if (CanGo(nextPos))
		{
			SetCellPos(nextPos);
			SetState(MOVE);
		}
	}
	else if (info.dir() == DIR_LEFT)
	{
		Vec2Int nextPos = GetCellPos() + deltaXY[info.dir()];
		if (CanGo(nextPos))
		{
			SetCellPos(nextPos);
			SetState(MOVE);
		}
	}
	else if (info.dir() == DIR_RIGHT)
	{
		Vec2Int nextPos = GetCellPos() + deltaXY[info.dir()];
		if (CanGo(nextPos))
		{
			SetCellPos(nextPos);
			SetState(MOVE);
		}
	}

	GET(NetMgr)->RegisterPacket(ClientPacketHandler::Make_C_Move());
}

void MyPlayer::TickIdle()
{
	TickInput();
	TryMove();
}

void MyPlayer::TickMove()
{
	Super::TickMove();
}

void MyPlayer::TickSkill()
{
	Super::TickSkill();
}