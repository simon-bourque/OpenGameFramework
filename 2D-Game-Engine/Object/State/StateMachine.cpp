#include "StateMachine.h"

#include "Object/State/State.h"
#include "Object/State/StateTransition.h"

StateMachine::StateMachine(State* initialState) : m_currentState(initialState) {}

StateMachine::~StateMachine() { 
	delete m_currentState;
}

void StateMachine::tick(float32 delta, Game* game) {
	transitionState();
	m_currentState->tick(delta, game);
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