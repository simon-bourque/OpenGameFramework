#pragma once

#include "Core/Core.h"

#include <type_traits>

struct Event
{
	enum class Type {
		DEFAULT,
		COLLISION_LEVEL,
		HIT_GROUND,
		APPLY_IMPULSE,
		FLIP_SPRITE,
		ANIM_STATE_CHANGE
	};

	Type type;
	
	union {
		int8 asInt8;
		uint8 asUInt8;
		int16 asInt16;
		uint16 asUInt16;
		int32 asInt32;
		uint32 asUInt32;
		float32 asFloat32;
		bool asBool;
		const void* asPointer;
	} param;

	Event();
	Event(Type type);
	Event(Type type, int8 param);
	Event(Type type, uint8 param);
	Event(Type type, int16 param);
	Event(Type type, uint16 param);
	Event(Type type, int32 param);
	Event(Type type, uint32 param);
	Event(Type type, float32 param);
	Event(Type type, bool param);
	Event(Type type, const void* param);
	virtual ~Event();

	template<typename T>
	const T* paramAsClass() const;
};

template<typename T>
const T* Event::paramAsClass() const {
	static_assert(std::is_class<T>::value, "Type must be a class.");
	return static_cast<const T*>(param.asPointer);
}
