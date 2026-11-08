#pragma once

#include "Actor.h"

class EventTrigger : public Actor
{
	using Super = Actor;
public:
	EventTrigger();
	virtual ~EventTrigger() override;
};
