#include "pch.h"
#include "ClientPacketHandler.h"
#include "BufferReader.h"
#include "GameScene.h"
#include "MyPlayer.h"
#include "SceneMgr.h"
#include "ObjMgr.h"
#include "GameObject.h"
#include "HitEffect.h"
#include "Scene.h"
#include "Arrow.h"
#include "Player.h"

/******* Server -> Client *******/

void ClientPacketHandler::HandlePacket(ServerSessionRef session, BYTE* buffer, int32 len)
{
	BufferReader br(buffer, len);

	PacketHeader header;
	br >> header;

	switch (header.id)
	{
	case S_EnterGame:
		Handle_S_EnterGame(session, buffer, len);
		break;
	case S_MyPlayer:
		Handle_S_MyPlayer(session, buffer, len);
		break;
	case S_AddObject:
		Handle_S_AddObject(session, buffer, len);
		break;
	case S_RemoveObject:
		Handle_S_RemoveObject(session, buffer, len);
		break;
	case S_Move:
		Handle_S_Move(session, buffer, len);
		break;
	case S_Attack:
		Handle_S_Attack(session, buffer, len);
		break;
	case S_Attack_Arrow:
		Handle_S_Attack_Arrow(session, buffer, len);
		break;
	case S_ObjectWeaponChange:
		Handle_S_ObjectWeaponChange(session, buffer, len);
		break;
	default:
		break;
	}
}

void ClientPacketHandler::Handle_S_TEST(ServerSessionRef session, BYTE* buffer, int32 len)
{
	PacketHeader* header = (PacketHeader*)buffer;
	//uint16 id = header->id;
	uint16 size = header->size;

	Protocol::S_TEST pkt;
	pkt.ParseFromArray(&header[1], size - sizeof(PacketHeader));

	uint64 id = pkt.id();
	uint32 hp = pkt.hp();
	uint16 attack = pkt.attack();

	//cout << "ID: " << id << " HP : " << hp << " ATT : " << attack << endl;

	for (int32 i = 0; i < pkt.buffs_size(); i++)
	{
		const Protocol::BuffData& data = pkt.buffs(i);
		//cout << "BuffInfo : " << data.buffid() << " " << data.remaintime() << endl;
	}
}

void ClientPacketHandler::Handle_S_EnterGame(ServerSessionRef session, BYTE* buffer, int32 len)
{
	PacketHeader* header = (PacketHeader*)buffer;
	//uint16 id = header->id;
	uint16 size = header->size;

	Protocol::S_EnterGame pkt;
	pkt.ParseFromArray(&header[1], size - sizeof(PacketHeader));

	bool success = pkt.success();
	uint64 accountId = pkt.accountid();

}

void ClientPacketHandler::Handle_S_MyPlayer(ServerSessionRef session, BYTE* buffer, int32 len)
{
	PacketHeader* header = (PacketHeader*)buffer;
	//uint16 id = header->id;
	uint16 size = header->size;

	Protocol::S_MyPlayer pkt;
	pkt.ParseFromArray(&header[1], size - sizeof(PacketHeader));

	//
	const Protocol::ObjectInfo& info = pkt.info();

	GameScene* scene = GET(SceneMgr)->GetGameScene();
	if (scene)
	{
		MyPlayer* myPlayer = scene->SpawnObject<MyPlayer>(Vec2Int{ info.posx(), info.posy() });
		myPlayer->info = info;
		myPlayer->SetName(info.name());

		Stat pStat = {};
		pStat.hp = info.hp();
		pStat.maxHp = info.maxhp();
		pStat.attack = info.attack();
		myPlayer->SetStat(pStat);

		GET(SceneMgr)->SetMyPlayer(myPlayer);
	}
}

void ClientPacketHandler::Handle_S_AddObject(ServerSessionRef session, BYTE* buffer, int32 len)
{
	PacketHeader* header = (PacketHeader*)buffer;
	//uint16 id = header->id;
	uint16 size = header->size;

	Protocol::S_AddObject pkt;
	pkt.ParseFromArray(&header[1], size - sizeof(PacketHeader));

	GameScene* scene = GET(SceneMgr)->GetGameScene();
	if (scene)
		scene->Handle_S_AddObject(pkt);
}

void ClientPacketHandler::Handle_S_RemoveObject(ServerSessionRef session, BYTE* buffer, int32 len)
{
	PacketHeader* header = (PacketHeader*)buffer;
	//uint16 id = header->id;
	uint16 size = header->size;

	Protocol::S_RemoveObject pkt;
	pkt.ParseFromArray(&header[1], size - sizeof(PacketHeader));

	GameScene* scene = GET(SceneMgr)->GetGameScene();
	if (scene)
		scene->Handle_S_RemoveObject(pkt);
}

void ClientPacketHandler::Handle_S_Move(ServerSessionRef session, BYTE* buffer, int32 len)
{
	PacketHeader* header = (PacketHeader*)buffer;
	//uint16 id = header->id;
	uint16 size = header->size;

	Protocol::S_Move pkt;
	pkt.ParseFromArray(&header[1], size - sizeof(PacketHeader));

	const Protocol::ObjectInfo& info = pkt.info();

	GameScene* scene = GET(SceneMgr)->GetGameScene();
	if (scene)
	{
		uint64 myPlayerId = GET(SceneMgr)->GetMyPlayerId();
		if (myPlayerId == info.objectid())
			return;

		GameObject* gameObject = scene->GetObject(info.objectid());
		if (gameObject)
		{
			gameObject->SetDir(info.dir());
			gameObject->SetState(info.state());
			gameObject->SetCellPos(Vec2Int{ info.posx(), info.posy() });
			gameObject->info = info;
		}
	}
}

void ClientPacketHandler::Handle_S_Attack(ServerSessionRef session, BYTE* buffer, int32 len)
{
	PacketHeader* header = (PacketHeader*)buffer;
	uint16 id = header->id;
	uint16 size = header->size;

	Protocol::S_Attack pkt;
	pkt.ParseFromArray(&header[1], size - sizeof(PacketHeader));

	const Protocol::ObjectInfo& info = pkt.info();

	GameScene* scene = GET(SceneMgr)->GetGameScene();
	if (scene)
	{
		uint64 myPlayerId = GET(SceneMgr)->GetMyPlayerId();
		if (myPlayerId == info.objectid())
			return;

		//GameObject* myPlayer = scene->GetObject(myPlayerId);
		GameObject* gameObject = scene->GetObject(info.objectid());
		if (gameObject)
		{
			gameObject->SetDir(info.dir());
			gameObject->SetState(info.state());
			gameObject->SetCellPos(Vec2Int{ info.posx(), info.posy() });
			gameObject->info = info;

			//myPlayer->SetState(SKILL);
		}
	}
}

void ClientPacketHandler::Handle_S_Attack_Arrow(ServerSessionRef session, BYTE* buffer, int32 len)
{
	PacketHeader* header = (PacketHeader*)buffer;
	uint16 id = header->id;
	uint16 size = header->size;

	Protocol::S_Attack_Arrow pkt;
	pkt.ParseFromArray(&header[1], size - sizeof(PacketHeader));

	const Protocol::AttackArrowInfo& info = pkt.info();

	GameScene* scene = (GameScene*)GET(SceneMgr)->GetCurrentScene();
	GameObject* creature = scene->GetObject(info.targetid());
	GameObject* player = scene->GetObject(info.objectid());

	Arrow* clientArrow = scene->SpawnObject<Arrow>(player->GetCellPos());
	clientArrow->SetHit(info.hitflag());
	clientArrow->SetDir(player->info.dir());

	player->SetState(SKILL);

	/*scene->SpawnObject<HitEffect>(creature->GetCellPos());
	((Creature*)creature)->OnDamaged(player);*/
}

void ClientPacketHandler::Handle_S_ObjectWeaponChange(ServerSessionRef session, BYTE* buffer, int32 len)
{
	PacketHeader* header = (PacketHeader*)buffer;
	uint16 id = header->id;
	uint16 size = header->size;

	Protocol::A_ObjectWeaponChange pkt;
	pkt.ParseFromArray(&header[1], size - sizeof(PacketHeader));

	uint64 objectId = pkt.objectid();
	WeaponType weaponType = pkt.state();

	GameScene* scene = (GameScene*)GET(SceneMgr)->GetCurrentScene();
	Player* gameObject = (Player*)scene->GetObject(objectId);
	if (gameObject)
	{
		gameObject->SetWeaponType(weaponType);
	}
}


/******* Client -> Server *******/

SendBufferRef ClientPacketHandler::Make_C_Move()
{
	Protocol::C_Move pkt;

	MyPlayer* myPlayer = GET(SceneMgr)->GetMyPlayer();

	*pkt.mutable_info() = myPlayer->info;

	return MakeSendBuffer(pkt, C_Move);
}

SendBufferRef ClientPacketHandler::Make_C_Attack(GameObject* target)
{
	Protocol::C_Attack pkt;

	MyPlayer* myPlayer = GET(SceneMgr)->GetMyPlayer();

	GameScene* scene = dynamic_cast<GameScene*>(GET(SceneMgr)->GetCurrentScene());
	if (scene == nullptr)
		return nullptr;

	pkt.mutable_info()->set_objectid(myPlayer->GetObjectID());
	pkt.mutable_info()->set_targetid(target->GetObjectID());
	pkt.mutable_info()->set_damege(myPlayer->info.attack());

	return MakeSendBuffer(pkt, C_Attack);
}

SendBufferRef ClientPacketHandler::Make_C_RemoveObject()
{
	Protocol::C_RemoveObject pkt;

	MyPlayer* myPlayer = GET(SceneMgr)->GetMyPlayer();

	pkt.add_ids(myPlayer->info.objectid());

	return MakeSendBuffer(pkt, C_RemoveObject);
}

SendBufferRef ClientPacketHandler::Make_C_Attack_Arrow(GameObject* target)
{
	Protocol::C_Attack_Arrow pkt;

	MyPlayer* myPlayer = GET(SceneMgr)->GetMyPlayer();

	pkt.mutable_info()->set_objectid(myPlayer->GetObjectID());
	if (nullptr == target)
		pkt.mutable_info()->set_targetid(-1);
	else
		pkt.mutable_info()->set_targetid(target->GetObjectID());
	pkt.mutable_info()->set_damege(myPlayer->info.attack());

	return MakeSendBuffer(pkt, C_Attack_Arrow);
}

SendBufferRef ClientPacketHandler::Make_C_ObjectWeaponChange(uint64 objectId, Protocol::OBJECT_WEAPON_TYPE type)
{
	Protocol::A_ObjectWeaponChange pkt;

	pkt.set_objectid(objectId);
	pkt.set_state(type);

	return MakeSendBuffer(pkt, C_ObjectWeaponChange);
}
