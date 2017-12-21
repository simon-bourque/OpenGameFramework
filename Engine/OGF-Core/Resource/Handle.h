#pragma once

#include "Core/Types.h"

#include "Core/EngineAssert.h"

template<typename T>
class Handle {
public:
	const static Handle<T> null;
private:
	uint64 m_uniqueID; // Currently unimplemented
	uint32* m_instanceCount;
	
	T* m_ptr;
public:
	Handle();
	explicit Handle(T* ptr);
	Handle(const Handle<T>& handle);
	Handle(const Handle<T>&& handle);
	virtual ~Handle();

	Handle<T>& operator=(const Handle<T>& handle);
	Handle<T>& operator=(const Handle<T>&& handle);

	// Dereference operators
	T& operator*();
	const T& operator*() const;
	T* operator->();
	const T* operator->() const;

	bool isNull() const;
	bool operator==(const Handle<T>& handle) const;
};

template<typename T>
const Handle<T> Handle<T>::null;

template<typename T>
Handle<T>::Handle() : m_uniqueID(0), m_instanceCount(nullptr), m_ptr(nullptr) {};

template<typename T>
Handle<T>::Handle(T* ptr) : m_uniqueID(0), m_instanceCount(new uint32(1)), m_ptr(ptr) {}

template<typename T>
Handle<T>::Handle(const Handle<T>& handle) : m_uniqueID(handle.m_uniqueID), m_ptr(handle.m_ptr), m_instanceCount(handle.m_instanceCount) {
	if (m_instanceCount) { // Don't increment a nullptr
		++(*m_instanceCount);
	}
}

template<typename T>
Handle<T>::Handle(const Handle<T>&& handle) : m_uniqueID(handle.m_uniqueID), m_ptr(handle.m_ptr), m_instanceCount(handle.m_instanceCount) {
	handle.m_instanceCount = nullptr;
	handle.m_ptr = nullptr;
}

template<typename T>
Handle<T>::~Handle() {
	if (m_instanceCount) {
		ASSERT(((*m_instanceCount) > 0), "Invalid handle state: \'(*m_instanceCount) <= 0\'.")
		if ((*m_instanceCount) == 1) {
			delete m_instanceCount;
			delete m_ptr;
		}
		else {
			--(*m_instanceCount);
		}
	}
}

template<typename T>
Handle<T>& Handle<T>::operator=(const Handle<T>& handle) {
	// Handle self assignment
	if (this == &handle) {
		return;
	}

	m_uniqueID = handle.m_uniqueID;
	m_ptr = handle.m_ptr;

	// Decrement old count and increment new one
	if (m_instanceCount) { // Only decrement if there was a count to start with
		--(*m_instanceCount);
	}

	m_instanceCount = handle.m_instanceCount;

	if (m_instanceCount) { // Don't inrement a nullptr
		++(*m_instanceCount);
	}

	return *this;
}

template<typename T>
Handle<T>& Handle<T>::operator=(const Handle<T>&& handle) {
	// Handle self assignment
	if (this == &handle) {
		return;
	}

	m_uniqueID = handle.m_uniqueID;
	m_ptr = handle.m_ptr;
	m_instanceCount = handle.m_instanceCount;

	handle.m_ptr = nullptr;
	handle.m_instanceCount = nullptr;

	return *this;
}

template<typename T>
T& Handle<T>::operator*() {
	return *m_ptr;
}

template<typename T>
const T& Handle<T>::operator*() const {
	return *m_ptr;
}

template<typename T>
T* Handle<T>::operator->() {
	return m_ptr;
}

template<typename T>
const T* Handle<T>::operator->() const {
	return m_ptr;
}

template<typename T>
bool Handle<T>::isNull() const {
	return ((*this) == null);
}

template<typename T>
bool Handle<T>::operator==(const Handle<T>& handle) const {
	return (m_uniqueID == handle.m_uniqueID) && (m_instanceCount == handle.m_instanceCount) && (m_ptr == handle.m_ptr);
}