#include "pch.h"
#include "NetMgr.h"
#include "Service.h"
#include "ThreadManager.h"
#include "ServerSession.h"
#include "Core.h"

NetMgr::NetMgr()
{

}

NetMgr::~NetMgr()
{

}

void NetMgr::Init()
{
	SocketUtils::Init();

	_service = make_shared<ClientService>(
		NetAddress(L"127.0.0.1", 8989),
		//NetAddress(L"211.58.26.46", 8989),
		make_shared<IocpCore>(),
		[=]() { return CreateSession(); },
		1);

	bool startResult = _service->Start();
	assert(startResult);

	/*for (int32 i = 0; i < 5; i++)
	{
		GThreadManager->Launch([=]()
			{
				while (true)
				{
					service->GetIocpCore()->Dispatch();
				}
			});
	}*/
}

void NetMgr::Update()
{
	_service->GetIocpCore()->Dispatch(0);

	SyncToServer();
}

void NetMgr::SyncToServer()
{
	if (!_session)
		return;

	if (_sendQueue.empty())
		return;

	while (!_sendQueue.empty())
	{
		SendBufferRef sendBuff = _sendQueue.front();
		_sendQueue.pop();

		_session->Send(sendBuff);
	}
}

ServerSessionRef NetMgr::CreateSession()
{
	return _session = make_shared<ServerSession>();
}

void NetMgr::RegisterPacket(SendBufferRef sendBuffer)
{
	if (sendBuffer != nullptr)
		_sendQueue.push(sendBuffer);
}
