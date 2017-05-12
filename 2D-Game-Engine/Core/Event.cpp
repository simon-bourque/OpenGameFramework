#include "Event.h"



Event::Event() : Event(Type::DEFAULT) {}

Event::Event(Type type) : type(type) {
	param.asPointer = nullptr;
}

Event::Event(Type type, int8 param) : type(type) {
	this->param.asInt8 = param;
}

Event::Event(Type type, uint8 param) : type(type) {
	this->param.asUInt8 = param;
}

Event::Event(Type type, int16 param) : type(type) {
	this->param.asInt16 = param;
}

Event::Event(Type type, uint16 param) : type(type) {
	this->param.asUInt16 = param;
}

Event::Event(Type type, int32 param) : type(type) {
	this->param.asInt32 = param;
}

Event::Event(Type type, uint32 param) : type(type) {
	this->param.asUInt32 = param;
}

Event::Event(Type type, float32 param) : type(type) {
	this->param.asFloat32 = param;
}

Event::Event(Type type, bool param) : type(type) {
	this->param.asBool = param;
}

Event::Event(Type type, const void* param) : type(type) {
	this->param.asPointer = param;
}

Event::~Event() {}
