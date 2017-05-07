#include "AnimatorComponent.h"

#include "Graphics/Animation/AnimState.h"

#include "Core/EngineAssert.h"

AnimatorComponent::AnimatorComponent(GameObject* parentObject, const string& currentStateName, AnimState* currentState) :
	RenderableComponent(parentObject),
	m_currentState(currentState),
	m_horizontalFlip(false),
	m_verticalFlip(false)
{
	m_states[currentStateName] = currentState;
}


AnimatorComponent::~AnimatorComponent() {
	for (const std::pair<string, AnimState*>& state : m_states) {
		delete state.second;
	}
}

void AnimatorComponent::changeState(const string& name) {
	auto it = m_states.find(name);

	if (it == m_states.end()) {
		ASSERT(false, "AnimState \"" + name + "\" does not exist. Add AnimState to the AnimatorComponent first.");
		return;
	}
	m_currentState = it->second;
	m_currentState->onEnter();
}

void AnimatorComponent::addState(const string& name, AnimState* state) {
	// TODO Warn user if they are replacing an animation
	m_states[name] = state;
}

void AnimatorComponent::removeState(const string& name) {
	auto it = m_states.find(name);

	if (it != m_states.end()) {
		m_states.erase(it);
	}
}

void AnimatorComponent::tick(float32 delta, Game* game) {
	m_currentState->tick(delta);
}

void AnimatorComponent::render(RenderSystem* rs) {
	m_currentState->render(rs, *this);
}
