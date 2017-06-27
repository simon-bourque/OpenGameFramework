#include "UIComponent.h"



UIComponent::UIComponent(const geo::Rectangle& bounds) : m_bounds(bounds), m_visible(true) {}

UIComponent::~UIComponent() {}
