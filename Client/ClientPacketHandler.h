#pragma once

class GameObject;

class ClientPacketHandler
{
public:
	static void HandlePacket(ServerSessionRef session, BYTE* buffer, int32 len);

	// 받기
	static void Handle_S_TEST(ServerSessionRef session, BYTE* buffer, int32 len);
	static void Handle_S_EnterGame(ServerSessionRef session, BYTE* buffer, int32 len);
	static void Handle_S_MyPlayer(ServerSessionRef session, BYTE* buffer, int32 len);
	static void Handle_S_AddObject(ServerSessionRef session, BYTE* buffer, int32 len);
	static void Handle_S_RemoveObject(ServerSessionRef session, BYTE* buffer, int32 len);
	static void Handle_S_Move(ServerSessionRef session, BYTE* buffer, int32 len);
	static void Handle_S_Attack(ServerSessionRef session, BYTE* buffer, int32 len);
	static void Handle_S_Attack_Arrow(ServerSessionRef session, BYTE* buffer, int32 len);
	static void Handle_S_ObjectWeaponChange(ServerSessionRef session, BYTE* buffer, int32 len);
	static void Handle_S_Buy(ServerSessionRef session, BYTE* buffer, int32 len);

	// 보내기
	static SendBufferRef Make_C_Move();
	static SendBufferRef Make_C_Attack(GameObject* target);
	static SendBufferRef Make_C_RemoveObject();
	static SendBufferRef Make_C_Attack_Arrow(GameObject* target);
	static SendBufferRef Make_C_ObjectWeaponChange(uint64 objectId, Protocol::OBJECT_WEAPON_TYPE type);
	static SendBufferRef Make_C_Buy(Protocol::ITEM_TYPE itemType);

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

