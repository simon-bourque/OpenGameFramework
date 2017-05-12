#pragma once
#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "Core/Core.h"

class State;
class Game;

class StateMachine {
private:
	State* m_currentState;

	void transitionState();
public:
	StateMachine(State* initialState);
	virtual ~StateMachine();

	void tick(float32 delta, Game* game);
};

#endif