#pragma once
#ifndef STATETRANSITION_H
#define STATETRANSITION_H

#include "Object/State/Condition.h"

class State;

class StateTransition {

	friend class StateMachine;

private:
	State* m_nextState;
	BaseCondition* m_condition;
public:
	StateTransition(State* nextState, BaseCondition* condition);
	virtual ~StateTransition();

	bool shouldTransition() const {
		return m_condition->eval();
	};

	template<typename T>
	static BaseCondition* createCondition(T* object, bool(T::*method)());
};

template<typename T>
static BaseCondition* StateTransition::createCondition(T* object, bool(T::*method)()) {
	return new Condition<T>(object, method);
}

#endif