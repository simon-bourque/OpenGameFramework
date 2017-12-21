#include "ObjectComponent.h"

ObjectComponent::ObjectComponent(GameObject* parentObject) : m_parentObject(parentObject), m_active(true) {}


ObjectComponent::~ObjectComponent() {}
