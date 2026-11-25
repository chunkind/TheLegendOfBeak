#pragma once
#include <mutex>
#include <atomic>

using int8 = __int8;
using int16 = __int16;
using int32 = __int32;
using int64 = __int64;
using uint8 = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;

template<typename T>
using Atomic = std::atomic<T>;
using Mutex = std::mutex;
using CondVar = std::condition_variable;
using UniqueLock = std::unique_lock<std::mutex>;
using LockGuard = std::lock_guard<std::mutex>;

// shared_ptr
using IocpCoreRef = std::shared_ptr<class IocpCore>;
using IocpObjectRef = std::shared_ptr<class IocpObject>;
using SessionRef = std::shared_ptr<class Session>;
using PacketSessionRef = std::shared_ptr<class PacketSession>;
using ListenerRef = std::shared_ptr<class Listener>;
using ServerServiceRef = std::shared_ptr<class ServerService>;
using ClientServiceRef = std::shared_ptr<class ClientService>;
using SendBufferRef = std::shared_ptr<class SendBuffer>;

#define size16(val)		static_cast<int16>(sizeof(val))
#define size32(val)		static_cast<int32>(sizeof(val))
#define len16(arr)		static_cast<int16>(sizeof(arr)/sizeof(arr[0]))
#define len32(arr)		static_cast<int32>(sizeof(arr)/sizeof(arr[0]))

enum
{
	S_TEST = 1,
	S_EnterGame = 2,

	S_MyPlayer = 4,
	S_AddObject = 5,

	C_RemoveObject = 6,
	S_RemoveObject = 7,

	C_Move = 10,
	S_Move = 11,

	C_Attack = 12,
	S_Attack = 13,

	C_Attack_Arrow = 14,
	S_Attack_Arrow = 15,

	C_ObjectWeaponChange = 16,
	S_ObjectWeaponChange = 17,

	C_Buy = 18,
	S_Buy = 19
};