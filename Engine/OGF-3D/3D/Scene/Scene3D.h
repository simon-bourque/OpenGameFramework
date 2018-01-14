#pragma once

#include "Core/Core.h"

#include <vector>
#include <memory>

//class GameObject;
//class Skybox;

class Scene3D {
private:

	//std::vector<GameObject*> m_objects;
	//Skybox m_skybox;

	// TODO PUT GRAVITY IN COLLISION/PHYSICS
	float32 m_gravity;
protected:
	void renderObjects();
	void renderBackgrounds();
public:
	Scene3D();
	virtual ~Scene3D();

	virtual void tick(float32 delta);
	virtual void render();

	//void addGameObject(GameObject* object);
	//void setSkybox(Skybox* skybox);

	float32 getGravity() const { return m_gravity; };
	void setGravity(float32 gravity) { m_gravity = gravity; };
};