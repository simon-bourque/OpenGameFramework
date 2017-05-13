#include "AnimatorComponent.h"

#include "Graphics/Animation/AnimState.h"

#include "Core/EngineAssert.h"
#include "Core/Event.h"

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

void AnimatorComponent::render() {
	m_currentState->render(*this);
}

void AnimatorComponent::receiveEvent(const Event& event) {
	switch (event.type) {
	case Event::Type::ANIM_STATE_CHANGE: {
		const string* name = static_cast<const string*>(event.param.asPointer);
		changeState(*name);
		break;
	}
	case Event::Type::FLIP_SPRITE: {
		m_horizontalFlip = event.param.asBool;
		break;
	}
	default:
		break;
	}
}
