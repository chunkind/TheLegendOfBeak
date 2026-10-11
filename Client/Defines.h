#pragma once

#define SING(type) public:			\
	static type* GetInst()			\
	{								\
		static type mgr;			\
		return &mgr;				\
	}								\
	private:						\
		type();						\
		~type();

#define GET(type)	type::GetInst()

#define SAFE_DELETE(ptr)			\
	if (ptr)						\
	{								\
		delete ptr;					\
		ptr = nullptr;				\
	}

#define DT TimeMgr::GetInst()->GetDeltaTime()