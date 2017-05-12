#include "StateTransition.h"


StateTransition::StateTransition(State* nextState, BaseCondition* condition) : m_nextState(nextState), m_condition(condition) {}

StateTransition::~StateTransition() {
	delete m_nextState;
	delete m_condition;
}
