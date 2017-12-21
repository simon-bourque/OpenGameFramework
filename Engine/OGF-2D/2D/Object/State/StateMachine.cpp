#include "StateMachine.h"

#include "2D/Object/State/State.h"
#include "2D/Object/State/StateTransition.h"

StateMachine::StateMachine() : m_currentState(nullptr) {}

StateMachine::~StateMachine() {
	for (const std::pair<string, State*>& pair : m_states) {
		delete pair.second;
	}
	for (const std::pair<string, StateTransition*>& pair : m_transitions) {
		delete pair.second;
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

void StateMachine::createTransition(const string& transitionName, const string& stateName, const string& nextStateName, BaseCondition* condition) {
	// TODO add assert or something similar for when trying to access states that don't exist
	auto stateIter = m_states.find(stateName);
	auto nextStateIter = m_states.find(nextStateName);

	if (stateIter != m_states.end() && nextStateIter != m_states.end()) {
		StateTransition* transition = new StateTransition((*nextStateIter).second, condition);
		m_transitions[transitionName] = transition;
		(*stateIter).second->addTransition(transition);
	}
}

void StateMachine::setCurrentState(const string& stateName) {
	// TODO add assert or something similar for when trying to access states that don't exist
	auto stateIter = m_states.find(stateName);

	if (stateIter != m_states.end()) {
		m_currentState = (*stateIter).second;
	}
}