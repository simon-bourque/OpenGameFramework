#pragma once

#include "PlayerState.h"

class PlayerController;

class WalkState : public PlayerState {
public:
	WalkState(PlayerController* pc);
	virtual ~WalkState();

	virtual void onEnter() override;
	virtual void onExit() override;
	virtual void tick(float32 delta) override;
};