#include "State.h"

State::State() {}

State::~State() {
	for (StateTransition* transition : m_transitions) {
		delete transition;
	}
}

void State::addTransition(StateTransition* transition) {
	m_transitions.push_back(transition);
}