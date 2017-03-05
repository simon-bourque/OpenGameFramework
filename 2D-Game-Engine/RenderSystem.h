#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "Core.h"
#include "Camera.h"

class RenderSystem
{
private:
	Camera m_camera;
public:
	RenderSystem(const Camera& camera);
	~RenderSystem();
};

#endif