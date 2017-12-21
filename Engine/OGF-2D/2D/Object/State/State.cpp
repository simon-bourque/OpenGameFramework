#include "State.h"

State::State() {}

State::~State() {}

void State::addTransition(StateTransition* transition) {
	m_transitions.push_back(transition);
}