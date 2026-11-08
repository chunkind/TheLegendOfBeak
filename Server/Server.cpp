#include "pch.h"
#include <iostream>
#include <thread>
#include <vector>
using namespace std;
#include <atomic>
#include <mutex>
#include "ThreadManager.h"
#include "SocketUtils.h"
#include "Listener.h"
#include "Service.h"
#include "GameSession.h"
#include "GameSessionManager.h"
#include "ServerPacketHandler.h"
#include "GameRoom.h"

int main()
{
	SocketUtils::Init();
	GRoom->Init();

	ServerServiceRef service = make_shared<ServerService>(
		NetAddress(L"127.0.0.1", 8989),
		make_shared<IocpCore>(),
		[]() { return make_shared<GameSession>(); },
		100);

	bool startResult = service->Start();
	assert(startResult);

	while (true)
	{
		service->GetIocpCore()->Dispatch(10);
		GRoom->Update();
	}

	GThreadManager->Join();

	// 윈속 종료
	SocketUtils::Clear();

}
