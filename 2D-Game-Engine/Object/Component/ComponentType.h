#pragma once

#include "Core/Core.h"

template<typename T>
struct ComponentType {
	static int64 id() { return -1; };
	static const string name() { return "UnregisteredType"; };
};

int64 getUniqueId();

#define REGISTER_COMPONENT_TYPE(T)\
template<>\
struct ComponentType<T> {\
	static int64 id() {\
		static int64 id = getUniqueId();\
		return id;\
	};\
	static const string name() { return #T; };\
};

#define GET_COMPONENT_TYPE_DECLERATION \
virtual int64 getTypeId() const override;

#define GET_COMPONENT_TYPE_DEFINITION(T)\
int64 T::getTypeId() const { return ComponentType<T>::id(); };
