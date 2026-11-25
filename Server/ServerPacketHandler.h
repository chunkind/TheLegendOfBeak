#pragma once

struct BuffData
{
	uint64 buffId;
	float remainTime;
};

class ServerPacketHandler
{
public:
	static void HandlePacket(GameSessionRef session, BYTE* buffer, int32 len);

	// 받기
	static void Handle_C_Move(GameSessionRef session, BYTE* buffer, int32 len);
	static void Handle_C_Attack(GameSessionRef session, BYTE* buffer, int32 len);
	static void Handle_C_RemoveObject(GameSessionRef session, BYTE* buffer, int32 len);
	static void Handle_C_Attack_Arrow(GameSessionRef session, BYTE* buffer, int32 len);
	static void Handle_C_ObjectWeaponChange(GameSessionRef session, BYTE* buffer, int32 len);
	static void Handle_C_Buy(GameSessionRef session, BYTE* buffer, int32 len);

	// 보내기
	static SendBufferRef Make_S_TEST(uint64 id, uint32 hp, uint16 attack, vector<BuffData> buffs);
	static SendBufferRef Make_S_EnterGame();
	static SendBufferRef Make_S_MyPlayer(const Protocol::ObjectInfo& info);
	static SendBufferRef Make_S_AddObject(const Protocol::S_AddObject& pkt);
	static SendBufferRef Make_S_RemoveObject(const Protocol::S_RemoveObject& pkt);
	static SendBufferRef Make_S_Move(const Protocol::ObjectInfo& info);
	static SendBufferRef Make_S_Attack(const Protocol::ObjectInfo& info);
	static SendBufferRef Make_S_Attack_Arrow(const Protocol::S_Attack_Arrow& info);
	static SendBufferRef Make_S_ObjectWeaponChange(int32 objectId, Protocol::OBJECT_WEAPON_TYPE state);
	static SendBufferRef Make_S_Buy(bool success, Protocol::ITEM_TYPE itemType, int32 money);

	template<typename T>
	static SendBufferRef MakeSendBuffer(T& pkt, uint16 pktId)
	{
		const uint16 dataSize = static_cast<uint16>(pkt.ByteSizeLong());
		const uint16 packetSize = dataSize + sizeof(PacketHeader);

		SendBufferRef sendBuffer = make_shared<SendBuffer>(packetSize);
		PacketHeader* header = reinterpret_cast<PacketHeader*>(sendBuffer->Buffer());
		header->size = packetSize;
		header->id = pktId;
		
		bool serializeResult = pkt.SerializeToArray(&header[1], dataSize);
		assert(serializeResult);
		sendBuffer->Close(packetSize);

		return sendBuffer;
	}
};