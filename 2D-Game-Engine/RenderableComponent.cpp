#include "RenderableComponent.h"



RenderableComponent::RenderableComponent(GameObject* parentObject) : ObjectComponent(parentObject), m_visible(true) {}


RenderableComponent::~RenderableComponent() {}
