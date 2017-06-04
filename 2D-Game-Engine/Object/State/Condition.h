#pragma once

class BaseCondition {
public:
	virtual bool eval() const = 0;
};

template<typename T>
class Condition : public BaseCondition {
private:
	T* m_object;
	bool(T::*m_method)();
public:
	Condition(T* object, bool(T::*method)());
	virtual bool eval() const override;
};

template <typename T>
Condition<T>::Condition(T* object, bool(T::*method)()) : m_object(object), m_method(method) {}

template<typename T>
bool Condition<T>::eval() const {
	return (m_object->*m_method)();
}