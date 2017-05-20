#pragma once

#include <vector>

class UIComponent;

class UserInterface {
private:
	std::vector<UIComponent*> m_components;
public:
	UserInterface();
	virtual ~UserInterface();

	void addUIComponent(UIComponent* component);

	void render();
};

