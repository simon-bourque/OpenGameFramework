#pragma once

#include "3D/Graphics/Shader/ShaderCache.h"
#include "3D/Graphics/Meshes/ModelCache.h"
#include "3D/Graphics/TextureCache.h"

#include "3D/Graphics/WaterRenderer.h"

#include "3D/Graphics/Camera.h"

class RenderingContext {
private: // Member variables
	static RenderingContext* s_instance;
public:
	Camera camera;

	ShaderCache shaderCache;
	ModelCache modelCache;
	TextureCache textureCache;
private: // Member functions
	RenderingContext();
	virtual ~RenderingContext();
public:
	void prepareFrame();
	void render();

	static void init() { s_instance = new RenderingContext(); };
	static RenderingContext* get() { return s_instance; };
	static void destroy() { delete s_instance; };
};