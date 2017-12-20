#pragma once

#include "Core/Core.h"

#include <unordered_map>
#include <type_traits>

class State;
class StateTransition;
class BaseCondition;
class Game;

class StateMachine {
private:
	State* m_currentState;

	std::unordered_map<string, State*> m_states;
	std::unordered_map<string, StateTransition*> m_transitions;

	void transitionState();
public:
	StateMachine();
	virtual ~StateMachine();

	void tick(float32 delta);

	template<typename T, typename... Params>
	void createState(const string& stateName, Params... args);
	
	void createTransition(const string& transitionName, const string& stateName, const string& nextStateName, BaseCondition* condition);

	void setCurrentState(const string& stateName);
};

template<typename T, typename... Params>
void StateMachine::createState(const string& stateName, Params... args) {
	static_assert(std::is_base_of<State, T>::value, "Type must derive from State.");
	
	State* state = new T(args...);

	m_states[stateName] = state;
};