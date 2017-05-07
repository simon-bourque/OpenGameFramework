#include "Renderer.h"

#include "Core/EngineAssert.h"

Renderer::Renderer(RenderSystem* rs) : m_rs(rs) {
	ASSERT(rs, "Rendersystem should not be null");
}


Renderer::~Renderer() {}
