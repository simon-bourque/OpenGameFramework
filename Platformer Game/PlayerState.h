#pragma once

#include "Object/State/State.h"

class PlayerController;

class PlayerState : public State {
protected:
	PlayerController* m_pc;
public:
	PlayerState(PlayerController* pc);
	virtual ~PlayerState();
};