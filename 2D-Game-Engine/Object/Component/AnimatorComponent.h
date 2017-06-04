#pragma once

#include "Core/Core.h"

#include "Object/Component/RenderableComponent.h"

#include "Object/Component/ComponentType.h"

#include <map>

class AnimState;
class GameObject;
class Game;

class AnimatorComponent : public RenderableComponent {
private:
	std::map<string, AnimState*> m_states;
	AnimState* m_currentState;
	
	bool m_horizontalFlip;
	bool m_verticalFlip;
public:
	AnimatorComponent(GameObject* parentObject, const string& currentStateName, AnimState* currentState);
	virtual ~AnimatorComponent();

	void changeState(const string& name);
	void addState(const string& name, AnimState* state);
	void removeState(const string& name);

	virtual void tick(float32 delta) override;
	virtual void render() override;

	bool isHorizontallyFlipped() const { return m_horizontalFlip; };
	bool isVerticallyFlipped() const { return m_verticalFlip; };

	void setHorizontalFlip(bool horizontalFlip) { m_horizontalFlip = horizontalFlip; };
	void setVerticalFlip(bool verticalFlip) { m_verticalFlip = verticalFlip; };

	virtual ComponentType getType() override { return ANIMATOR_COMPONENT; };
	virtual void receiveEvent(const Event& event) override;

#ifdef DEBUG_BUILD
	virtual void debugRender() override;
#endif
};