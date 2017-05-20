#include "UserInterface.h"

#include "UIComponent.h"


UserInterface::UserInterface() {}


UserInterface::~UserInterface() {
	for (UIComponent* component : m_components) {
		delete component;
	}
}

void UserInterface::addUIComponent(UIComponent* component) {
	m_components.push_back(component);
}

void UserInterface::render() {
	for (UIComponent* component : m_components) {
		component->render();
	}
}
