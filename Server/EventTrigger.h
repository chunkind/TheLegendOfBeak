#pragma once

#include "GameObject.h"

class EventTrigger : public GameObject
{
	using Super = GameObject;

public:
	EventTrigger();
	virtual ~EventTrigger() override;
};
