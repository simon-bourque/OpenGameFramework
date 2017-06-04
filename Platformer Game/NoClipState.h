#ifdef DEBUG_BUILD
#pragma once

#include "Core/Core.h"

#include "PlayerState.h"

class PlayerController;

class NoClipState : public PlayerState {
public:
	NoClipState(PlayerController* pc);
	virtual ~NoClipState();

	virtual void onEnter() override;
	virtual void onExit() override;
	virtual void tick(float32 delta) override;
};

#endif