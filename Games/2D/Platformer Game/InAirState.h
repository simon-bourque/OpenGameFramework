#pragma once

#include "PlayerState.h"

class PlayerController;

class InAirState : public PlayerState {

public:
	InAirState(PlayerController* pc);
	virtual ~InAirState();

	void onEnter() override;
	void onExit() override;
	void tick(float32 delta) override;
};