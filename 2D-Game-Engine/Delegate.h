#pragma once
#ifndef DELEGATE_H
#define DELEGATE_H

template <typename... Params>
class BaseDelegate {
public:
	virtual void operator()(Params... args) = 0;
};

template <typename T, typename... Params>
class Delegate : public BaseDelegate<Params...> {
private:
	T* m_object;
	void(T::*m_method)(Params...);

public:
	Delegate(T* object, void(T::*method)(Params...));
	virtual void operator()(Params... args) override;
};

template <typename T, typename... Params>
Delegate<T, Params...>::Delegate(T* object, void(T::*method)(Params...)) : m_object(object), m_method(method) {}

template <typename T, typename... Params>
void Delegate<T, Params...>::operator()(Params... args) {
	(m_object->*m_method)(args...);
}

#endif