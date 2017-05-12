#pragma once
#ifndef IDLESTATE_H
#define IDLESTATE_H

#include "PlayerState.h"

class PlayerController;

class IdleState : public PlayerState {
public:
	IdleState(PlayerController* pc);
	virtual ~IdleState();

	virtual void onEnter() override;
	virtual void onExit() override;
	virtual void tick(float32 delta, Game* game) override;
};

#endif