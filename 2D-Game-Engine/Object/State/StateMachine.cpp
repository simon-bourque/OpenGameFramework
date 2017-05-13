#include "StateMachine.h"

#include "Object/State/State.h"
#include "Object/State/StateTransition.h"

StateMachine::StateMachine(State* initialState) : m_currentState(initialState) {}

StateMachine::~StateMachine() { 
	for (State* state : m_states) {
		delete state;
	}
	for (StateTransition* transition : m_transtions) {
		delete transition;
	}
}

void StateMachine::tick(float32 delta) {
	transitionState();
	m_currentState->tick(delta);
}

void StateMachine::transitionState() {
	for (StateTransition* transtion : m_currentState->m_transitions) {
		if (transtion->shouldTransition()) {
			m_currentState->onExit();
			m_currentState = transtion->m_nextState;
			m_currentState->onEnter();
			break;
		}
	}
}

void StateMachine::addState(State* state) {
	m_states.push_back(state);
}

void StateMachine::addTransition(StateTransition* transition) {
	m_transtions.push_back(transition);
}