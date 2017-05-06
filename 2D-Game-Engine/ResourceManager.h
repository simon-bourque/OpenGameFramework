#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "Core.h"
#include <unordered_map>

class RenderSystem;

template <typename T>
class ResourceManager {
protected:
	RenderSystem* m_rs;
	std::unordered_map<string, T*> m_loadedResources;
public:
	ResourceManager(RenderSystem* rs);
	virtual ~ResourceManager();
	
	void deleteResource(const string& name);
	void clear();
	T* getResource(const string& name) const;

	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;
};

template <typename T>
ResourceManager<T>::ResourceManager(RenderSystem* rs) : m_rs(rs) {}

template <typename T>
ResourceManager<T>::~ResourceManager() {
	for (const std::pair<string, T*>& pair : m_loadedResources) {
		delete pair.second;
	}
}

template <typename T>
void ResourceManager<T>::deleteResource(const string& name) {
	auto it = m_loadedResources.find(name);
	if (it != m_loadedResources.end()) {
		delete it->second;
		m_loadedResources.erase(it);
	}
}

template <typename T>
void ResourceManager<T>::clear() {
	for (const std::pair<string, T*>& pair : m_loadedResources) {
		delete pair.second;
	}
	m_loadedResources.clear();
}

template <typename T>
T* ResourceManager<T>::getResource(const string& name) const {
	auto it = m_loadedResources.find(name);
	if (it != m_loadedResources.end()) {
		return it->second;
	}
	return nullptr;
}


#endif