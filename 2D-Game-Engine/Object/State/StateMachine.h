#pragma once
#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "Core/Core.h"

#include <vector>

class State;
class StateTransition;
class Game;

class StateMachine {
private:
	State* m_currentState;

	std::vector<State*> m_states;
	std::vector<StateTransition*> m_transtions;

	void transitionState();
public:
	StateMachine(State* initialState);
	virtual ~StateMachine();

	void tick(float32 delta);

	void addState(State* state);
	void addTransition(StateTransition* transition);
};

#endif