#pragma once

template <typename... Params>
class Delegate {
private:
	void* m_object;
	void(*m_function)(void*, Params...);

	template <typename void(*FUNCTION)(Params...)>
	static void freeFunctionCaller(void* object, Params... args) {
		FUNCTION(args...);
	};

	template<typename T, typename void(T::*FUNCTION)(Params...)>
	static void  memberFunctionCaller(void* object, Params... args) {
		(static_cast<T*>(object)->*FUNCTION)(args...);
	};

	Delegate(void* object, void(*function)(void*, Params...));
public:
	Delegate();
	template<typename T, typename void(T::*FUNCTION)(Params...)>
	static Delegate create(T* object);

	template<typename void(*FUNCTION)(Params...)>
	static Delegate create();

	template<typename T, typename void(T::*FUNCTION)(Params...)>
	void bind(T* object);

	template<typename void(*FUNCTION)(Params...)>
	void bind();

	void operator()(Params... args);

};

template <typename... Params>
Delegate<Params...>::Delegate() : m_object(nullptr), m_function(nullptr) {}

template <typename... Params>
Delegate<Params...>::Delegate(void* object, void(*function)(void*, Params...)) : m_object(object), m_function(function) {}

template <typename... Params>
template <typename T, typename void(T::*FUNCTION)(Params...)>
Delegate<Params...> Delegate<Params...>::create(T* object) {
	return {object, &memberFunctionCaller<T, FUNCTION>};
}

template <typename... Params>
template<typename void(*FUNCTION)(Params...)>
Delegate<Params...> Delegate<Params...>::create() {
	return {nullptr, &freeFunctionCaller<FUNCTION>};
}

template <typename... Params>
template<typename T, typename void(T::*FUNCTION)(Params...)>
void Delegate<Params...>::bind(T* object) {
	m_object = object;
	m_function = &memberFunctionCaller<T, FUNCTION>;
}

template <typename... Params>
template<typename void(*FUNCTION)(Params...)>
void Delegate<Params...>::bind() {
	m_object = nullptr;
	m_function = &freeFunctionCaller<FIUNCTION>;
}

template <typename... Params>
void Delegate<Params...>::operator()(Params... args) {
	if (m_function) {
		m_function(m_object, args...);
	}
}