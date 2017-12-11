#pragma once

template<typename T>
class Singleton {
private:
	static T* s_instance;

	Singleton() {}
public:
	virtual ~Singleton() {}

	static T* get() {
		ASSERT(s_instance, "Singleton must be initialized before use.");
		return s_instance;
	};

	template<typename... Params>
	static void init(Params... args) {
		ASSERT(!s_instance, "Singleton is already initialized.");
		s_instance = new T(args...);
	}

	static bool isInitialized() {
		return (s_instance != nullptr);
	}

	static void destroy() {
		delete s_instance;
	}
};

template<typename T>
T* Singleton<T>::s_instance = nullptr;
